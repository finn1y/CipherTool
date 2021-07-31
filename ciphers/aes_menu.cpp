#include "aes_menu.h"
#include "finn_menu.h"

// the standard menu class functions are the same as for any derived menu class

aes_menu::aes_menu(bool& runState, menuPtr& currentMenu): _runState(runState), _currentMenu(currentMenu) {
    // initialise private members
    this->_encryption = true;
    this->_aes = NULL; // null pointer by default to prevent any potential issues before pointer is set to object
}

aes_menu::~aes_menu() {}

void aes_menu::update() {
    this->print();

    int input = this->handle_int("\nEnter desired menu option: ", 5);

    switch (input) {
        case 1:
            this->_encryption = true;
            this->aes_file(); // aes cipher on input from files and outputting to files
            this->exit(); // after carrying out an aes cipher operation the application will give the user the option to exit the application
            break;

        case 2:
            this->_encryption = false;
            this->aes_file(); // aes cipher on user input from the console
            this->exit();
            break;

        case 3:
            this->_encryption = true;
            this->aes_input();
            this->exit();
            break;

        case 4:
            this->_encryption = false;
            this->aes_input();
            this->exit();
            break;

        case 5:
            this->_currentMenu = menuPtr(new finn_menu(this->_runState, this->_currentMenu));
            break;
    }

    // clean up
    this->_bytes.clear(); // clear bytes vector
}

void aes_menu::print() {
    std::cout << "\n**** Rjindael's Cipher (AES) ****" << std::endl;
    std::cout << "|-------------------------------|" << std::endl;
    std::cout << "|   1: File encryption\t\t|" << std::endl;
    std::cout << "|   2: File decryption\t\t|" << std::endl;
    std::cout << "|   3: Input encryption\t\t|" << std::endl;
    std::cout << "|   4: Input decryption\t\t|" << std::endl;
    std::cout << "|   5: Back\t\t\t|" << std::endl;
    std::cout << "|-------------------------------|" << std::endl; 
}

// aes cipher on input from a file and outputted to a file
void aes_menu::aes_file() {
    std::string file_name;
    bool valid_file = false;

    do {
        // user is prompted to input the file name
        file_name = this->handle_input("\nEnter file name: ");

        // the file name is checked for the .txt extension
        if (file_name.find(".txt") == size_t(-1)) {
            file_name.append(".txt"); // if the file name is missing the extension it will be added
        }

        std::ifstream in_file(file_name);
        // open the file and read the data
        if (in_file.is_open()) {
            if (this->_encryption) {
                // get the size of the file
                int size = this->get_file_size(file_name);
                int counter = 0;
                while (!in_file.eof()) {
                    std::string line{};
                    // each line is read individually to get any formatting, e.g. whitespace and tabs
                    std::getline(in_file, line);
                    counter++; // a counter is incremented
                    // each byte is added to the bytes vector individually
                    for (size_t i = 0; i < line.size(); i++) { 
                        this->_bytes.push_back(line[i]);
                    }
                    // the new line character should not be added on the final line
                    if (!(counter == size)) {
                        // the new line character is excluded by the getline function so it must be added manually
                        this->_bytes.push_back(10); // the decimal value for the new line character is 10
                    }
                }
                valid_file = true;
            }
            if (!this->_encryption) {
                // the file is attempted to be read as if written in hexadecimal
                try {
                    while(!in_file.eof()) {
                        std::string line{};
                        std::getline(in_file, line);

                        // each hex value is read from the current line of the input file
                        for (size_t i = 0; i < line.size(); i+=2) {
                            std::string hex_val = line.substr(i, 2); // a substring of the line is taken
                            uint8_t byte = std::stoi(hex_val, 0, 16); // the substring in converted into an int
                            this->_bytes.push_back(byte);
                        }
                    }
                    valid_file = true;
                } 
                // if the file is not written in hexadecimal then an invalid argument exception is thrown from the string to int function
                catch (const std::invalid_argument& invalid_argument) {
                    std::cout << "Invalid file format! Please ensure file is encrypted in hexadecimal..." << std::endl;
                }   
            }
            // end of file reached therefore close file and exit the loop
            in_file.close();
        }
        else {
            std::cout << "\nFile not found! ";
            bool return_to_menu = false;

            // if file not found the user can return to the menu
            // without this the user may be stuck in an infinite loop asking for a file name they don't have
            do {
                std::string input_str = this->handle_input("Return to the menu?(y/n): ");
                if (input_str == "y" || input_str == "Y") {
                    return;
                }
                else if (input_str == "n" || input_str == "N") {
                    return_to_menu = true;
                }
                else {
                    std::cout << "Invalid input! Please enter a valid input..." << std::endl;
                }
            } while (!return_to_menu);
        }
    } while (!valid_file);

    this->_aes = new aes(this->_bytes);
  
    // a key is gotten from the user
    if (!this->get_key()) {
        return; // if no key is gotten the function is exited early
    }  

    if (this->_encryption) {
        this->_aes->encrypt();
        std::cout << "File successfully encrypted!" << std::endl; // user feedback that the encryption was successful
    }
    if (!this->_encryption) {
        this->_aes->decrypt();
        std::cout << "File successfully decrypted!" << std::endl; // user feedback that the decryption was successful
    }

    std::string new_file_name = file_name;
    bool valid_overwrite = false;

    // a file name for the output is gotten from the user the default is the same name, i.e. overwrite
    do {
        std::string overwrite = this->handle_input("\nDo you want to overwite the file?(y/n): ");
        if (overwrite == "y" || overwrite == "Y") {
            valid_overwrite = true; // if the user wishes to overwrite new file name is kept as default
        }
        else if (overwrite == "n" || overwrite == "N") {
            new_file_name = this->handle_input("\nEnter new file name: "); // if the user doesn't wish to overwrite a new file name is input by the user
            valid_overwrite = true;
        }
        else {
            std::cout << "Invalid input! Please enter a valid input..." << std::endl;
        }
    } while (!valid_overwrite);

    // the new file name is checked for the .txt extension
    if (new_file_name.find(".txt") == size_t(-1)) {
        new_file_name.append(".txt"); // if it doesn't have the extension it is added
    }

    std::ofstream out_file(new_file_name);

    if (out_file.is_open()) {
        if (this->_encryption) {
            // the encrypted bytes vector is written to the output file
            // the output is written in hexadecimal for user readability
            for (size_t i = 0; i < this->_bytes.size(); i++) {
                out_file << std::hex << std::setw(2) << std::setfill('0') << int(this->_bytes[i]) << std::dec;
            }
        }
        if (!this->_encryption) {
            // the decrypted bytes vector is written to the output file
            // this is plaintext so does not need to be printed in hexadecimal
            for (size_t i = 0; i < this->_bytes.size(); i++) {
                out_file << this->_bytes[i];
            }
        }
    }
}

// aes cipher on input from the console outputted to the console
void aes_menu::aes_input() {
    bool valid_input = false;
    std::string input_str;

    do {
        if (this->_encryption) { 
            // the user is promted to enter the plaintext
            input_str = this->handle_input("\nEnter plaintext: ");
            if (!input_str.empty()) {
                // the user inputted string is transformed into the vector of bytes to be used in the xor object
                this->_bytes.resize(input_str.size());
                for (size_t i = 0; i < input_str.size(); i++) {
                    this->_bytes[i] = input_str.at(i);
                }
                valid_input = true;
            }
            else {
                std::cout << "Invalid input! Input cannot be an empty string..." << std::endl;
            }
        }
        if (!this->_encryption) {
            try {
                // the user is prompted to enter the ciphertext
                input_str = this->handle_input("\nEnter ciphertext: ");
                // the cipher text is converted into the vector of bytes
                for (size_t i = 0; i < input_str.size(); i+=2) {
                    std::string sub_str = input_str.substr(i, 2);
                    uint8_t byte = std::stoi(sub_str, 0, 16);
                    this->_bytes.push_back(byte);
                }
                valid_input = true;
            }
            catch (const std::invalid_argument& invalid_argument) { // the input for decrpyiton must be written in hexadecimal format
                std::cout << "Invalid input! Input must be in hexadecimal format..." << std::endl;
            }
        }
    } while (!valid_input);
    
    // the aes object is created passing in the vector of bytes 
    this->_aes = new aes(this->_bytes);
  
    // a key is gotten from the user
    if (!this->get_key()) {
        return; // if no key is gotten the function is exited early
    }  

    // the input is then encrypted or decrypted and printed to the command line
    if (this->_encryption) {
        this->_aes->encrypt(); // aes encryption is carried out
        std::cout << "Ciphertext: ";
        // ciphertext is printed in hexadecimal for readability
        for (size_t i = 0; i < this->_bytes.size(); i++) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << int(this->_bytes[i]) << std::dec;
        }
    }
    if (!this->_encryption) {
        this->_aes->decrypt(); // aes decryption is carried out
        std::cout << "Plaintext: ";
        for (size_t i = 0; i < this->_bytes.size(); i++) {
            std::cout << this->_bytes[i];
        }
    }
    std::cout << std::endl;
}

// function to exit the application directly from this menu
void aes_menu::exit() {
    bool cont = false;

    // loop for getting a valid user input
    do {
        std::string exit = this->handle_input("\nExit?(y/n): ");
        if (exit == "y" || exit == "Y") {
            this->_runState = false; // if the user want to they can exit the application
            cont = true;
        }
        else if (exit == "n" || exit == "N") {
            cont = true; // if they do not want to the application will continue and tis menu will re-run
        }
        else {
            std::cout << "Invalid input! Please enter a valid input..." << std::endl;
       }
    } while (!cont);
}

// function to get a valid key from the user
bool aes_menu::get_key() {
    bool valid_key = false;

    do {
        std::string input_key = this->handle_input("\nDo you have a key?(y/n): ");
        if (input_key == "y" || input_key == "Y") {
            try {
                this->_aes->set_key(this->handle_input("\nEnter key: ")); // if the user has a key they input it
                valid_key = true;
            }
            catch (const std::out_of_range& out_of_range) { // if the key is not valid (empty string) out of range exception is thrown
                std::cout << "Invalid key! Key must contain at least one character..." << std::endl;
            }
            catch (const std::invalid_argument& invalid_arguemnt) { // if the key it too large an invalid argument exception is thrown
                std::cout << "Invalid key! Key must not be more than 32 characters long..." << std::endl;
            }
        }
        else if (input_key == "n" || input_key == "N") {
            if (this->_encryption) {
                this->_aes->gen_key(); // if the user doesn't have a key a random key is generated
                valid_key = true;
            }
            if (!this->_encryption) {
                // if the user doesn't have a key the file cannot be decrypted
                std::cout << "\nUnable to decrypt without the key!" << std::endl;
                return valid_key; // the function will return false if no key is gotten 
            }
        }
        else {
            std::cout << "Invalid input! Please enter a valid input..." << std::endl;
        }
    } while (!valid_key);

    // function will return true if a valid key is gotten
    return valid_key;
}

// function to count the number of lines in a file
int aes_menu::get_file_size(std::string file_name) {
    int count = 0;
    std::ifstream file(file_name);

    if (file.is_open()) {
        std::string line{};
        // read the lines of the file until the end of the file
        while (!file.eof()) {
            std::getline(file, line);
            // every time a line is read the counter increments
            count++;
        }
        // once the end of the file is reached close the file
        file.close();
        count--; // the count must be decremented as the final loop increments for a false line
    }

    return count;
}
