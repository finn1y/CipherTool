#include "jj_menu.h"
#include "main_menu.h"

#include <iostream>
#include <fstream>
#include <random>
#include <time.h>
#include "../ciphers/otp.h"

jj_menu::jj_menu(bool& runState, menuPtr& currentMenu): _runState(runState), _currentMenu(currentMenu) {}

jj_menu::~jj_menu() {}



void jj_menu::update() {

	
    this->print();

    int input = this->handle_int("\nEnter desired menu option: ", 4);

    switch (input) {
        case 1:
			//Decrypt
			//Clear current variables in case of repeat use:
			otp.encodedString = "";
			otp.key = "";
			
			//Take user input for the name of the file containing encrypted text
			while (otp.encodedString == "") {
				std::cout << "Encrypted text: From file" << std::endl;
				otp.encodedString = getFromFile(getFileName(), false);
			}

			//Take user input for the name of the file containing the key
			while (otp.key == "") {
				std::cout << "Key: From file" << std::endl;
				otp.key = getFromFile(getFileName(), true);
			}

			//ensure the key is long enough to decrypt entire encrypted message
			checkKeyLength(otp.key, otp.encodedString);

			//Decrypt encrypted text
			otp.decodedString = OTPDecode(otp.encodedString, otp.key);
			outputToFile(otp.decodedString);
            break;
        
        case 2:
			//Encrypt
			otp.decodedString = "";
			otp.key = "";

			while (otp.decodedString == "") {
				std::cout << "Plain text: From file" << std::endl;
				otp.decodedString = getFromFile(getFileName(), false);
			}

			while (otp.key == "") {
				std::cout << "Key: From file" << std::endl;
				otp.key = getFromFile(getFileName(), true);
			}

			checkKeyLength(otp.key, otp.decodedString);

			otp.encodedString = OTPEncode(otp.decodedString, otp.key);
			outputToFile(otp.encodedString);

            break;

        case 3:
			otp.generatedKey = "";
			otp.generatedKey = generateKey();
			std::cout << otp.generatedKey;
			outputToFile(otp.generatedKey);

            break;

		case 4:
			// goes back to previous menu
			this->_currentMenu = menuPtr(new main_menu(this->_runState, this->_currentMenu));
			break;
    }
};

//Print user interface
void jj_menu::print() {
    /* temporary menu for debugging */
	
    std::cout << "\n*****One-Time Pad*****" << std::endl;
    std::cout << "|---------------------|" << std::endl;
    std::cout << "|   1: Decrypt        |" << std::endl;
    std::cout << "|   2: Encrypt        |" << std::endl;
	std::cout << "|   3: Generate Key   |" << std::endl;
    std::cout << "|   4: Back           |" << std::endl;
    std::cout << "|---------------------|" << std::endl;
}

//Decrypt encrypted text
std::string jj_menu::OTPDecode(std::string encodedString, std::string key) {

	//Clear and set variables for the output - The decrypted text
	std::string output = "";

	
	int keyCounter = 0;
	//Iterate through each character in the encrypted text
	for (size_t i = 0; i < encodedString.length(); i++) {

		//If the character is not alphanumeric (e.g symbol or special character), add it directly to output
		if (otp.acceptedChar.find(encodedString[i]) == -1) {
			output.push_back(encodedString[i]);
		}
		//If the character is alphanumeric use the onetime pad in otp.h to decrypt
		else {
			std::string encodedTemp = "";
			encodedTemp.push_back(encodedString[i]);

			std::string keyTemp = "";
			keyTemp.push_back(key[keyCounter]);
			//Move to next character in key (prevents program from trying to decrypt unnecessary characters, such as
			//symbols and special characters
			keyCounter++;

			//Add each individual decrypted character to the output string
			output.push_back(otp.decodeChar(encodedTemp, keyTemp));
		}
	}

	//Print the decrypted text to screen and return it
	std::cout << output;
	return output;
}

//Encrypt plain text
std::string jj_menu::OTPEncode(std::string decodedString, std::string key) {


	//Clear and set variables for the output - The encrypted text
	std::string output = "";

	int keyCounter = 0;
	//Iterate through each character in the plain text
	for (size_t i = 0; i < decodedString.length(); i++) {
		
		//If the character is not alphanumeric (e.g symbol or special character), add it directly to output
		if (otp.acceptedChar.find(decodedString[i]) == -1) {
			output.push_back(decodedString[i]);

		}
		
		//If the character is alphanumeric use the onetime pad in otp.h to encrypt
		else {
			std::string decodedTemp = "";
			decodedTemp.push_back(decodedString[i]);

			std::string keyTemp = "";
			keyTemp.push_back(key[keyCounter]);

			//Move to next character in key (prevents program from trying to encrypt unnecessary characters, such as
			//symbols and special characters
			keyCounter++;

			//Add each individual encrypted character to the output string
			output.push_back(otp.encodeChar(decodedTemp, keyTemp));
		}
		
	}

	
	std::cout << output;
	return output;
}

//Get text from a file, given the user submitted file name
std::string jj_menu::getFromFile(std::string fileName, bool isKey) {
		
	std::ifstream userFile;

	//If file doesn't open, return a empty string
	//This is used to repeat input until correct file is found
	//Also warns user that the file may be empty
	userFile.open(fileName);
	if (!userFile.is_open()) {
		std::cerr << "No input (or empty) file found" << std::endl;
		return "";
	}

	//Add each line of the file to a string 
	std::string fileLine;
	std::string userFileString = "";

	//Add a linebreak character at the end of section of text in string
	while (std::getline(userFile, fileLine)) {
		userFileString += fileLine + "\n";
	}
	
	//Remove any special characters from key string, including blank spaces
	//The key should only be alphanumeric
	if (isKey == true) {
		for (size_t i = 0; i < userFileString.length(); i++) {
			if (otp.acceptedChar.find(userFileString[i]) == -1) {
				userFileString.erase(i,1); 
			}

		}
	}
	
	return userFileString;
	
}

//Ask for user input in selecting a file
std::string jj_menu::getFileName() {

	std::string fileName;
	std::cout << "Enter the name of a file: ";
	std::cin >> fileName;

	//Check whether the user has defined the file extension
	//If not, add a .txt file extension
	if (fileName.length() > 4) {

		std::string fileExt = fileName.substr(fileName.length() - 4);

		if (fileExt != ".txt") {
			fileName += ".txt";
		}
	}
	else {
		fileName += ".txt";
	}

	return fileName;

}

//Check that the key is long enough to decrypt the entire text.
void jj_menu::checkKeyLength(std::string key, std::string encodedString) {

	//If the key is too short, only decrypt the text to the maximum length of the string
	if (key.length() < encodedString.length()) {
		std::cout << "Key is too short for encrypted file - Decryption may be trunkated" << std::endl;
		otp.encodedString = encodedString.substr(0, key.length());

	}

	//Remove redundant extra length to key if it is not necessary.
	if (key.length() > encodedString.length()) {
		otp.key = key.substr(0, encodedString.length());
	}
}

void jj_menu::outputToFile(std::string output) {
	bool userDecision = false;
	std::string userInput;

	while (userDecision == false) {
		std::cout << "Would you like to output the result to a file? (Y/N): ";
		std::cin >> userInput;

		if (userInput[0] == 'Y' || userInput[0] == 'y') {
			std::cout << "What would you like to name the file?: ";
			std::string fileName;
			std::cin >> fileName;

			if (fileName.length() > 4) {

				std::string fileExt = fileName.substr(fileName.length() - 4);

				if (fileExt != ".txt") {
					fileName += ".txt";
				}
			}
			else {
				fileName += ".txt";
			}

			std::ofstream userFile(fileName);
			userFile << output;
			userFile.close();

			userDecision = true;

		}

		else {
			userDecision = true;
		}

	}

}

std::string jj_menu::generateKey() {

	std::string outputKey = "";
	std::string keyLength;

	bool valid_input = false;
	int input;

	//Exception handling for taking an integer input, from menu.cpp
	// do while loop to get a valid input from the user
	do {
		std::string input_str = this->handle_input("Enter the length of the key you require... (Key must be equal or longer than text you wish to encrypt/decrypt): ");

		// exception handling for string to int function
		// if the input is not a valid int then the string to int function will return an excpetion
		try {
			input = std::stoi(input_str); // the user input is attempted to be converted to an int
			valid_input = true; // if the user input is valid exit the loop
		}
		// string to int function will only throw invalid argument and out of range exceptions [see documentation] 
		catch (const std::invalid_argument& invalid_argument) { // if an invalid argument exception is thrown this block runs
			std::cout << "Invalid input! Please enter an integer value..." << std::endl;
		}
		catch (const std::out_of_range& out_of_range) { // if an out of range exception is thrown this block runs
			std::cout << "Invalid input! Please enter an integer in the range of 0 to 2147483647..." << std::endl;
		}
	} while (!valid_input);

	srand(time(NULL));

	for (int i = 0; i < input; i++) {
		int randInt = rand() % 36;
		outputKey.push_back(otp.alphaNum[randInt]);
	}

	return outputKey;
}
