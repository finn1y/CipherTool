#include "menu.h"

// only the non pure vitrual functions need to be declared
// destructor needs to be declared but will be re-defined by smart pointer destructor as virtual
menu::~menu() {}

// the handle input function will be inherited by all derived menu classes for them to use as needed
std::string menu::handle_input(std::string str) {
        std::string input{};

    std::cout << str;
    std::getline(std::cin, input);

    return input;
}

// handle int function used to get a valid integer input within a range of 0 to n exclusive
int menu::handle_int(std::string str, int n) {
    bool valid_input = false;
    int input;

    // do while loop to get a valid input from the user
    do {
        std::string input_str = this->handle_input(str);

        // exception handling for string to int function
        // if the input is not a valid int then the string to int function will return an excpetion
        try {
            input = std::stoi(input_str); // the user input is attempted to be converted to an int
            if (input > 0 && input <= n) {
                valid_input = true; // if the user input is valid exit the loop
            }
            else {
                throw std::invalid_argument("Invalid input"); // if the input is not valid an exception is thrown of the same type as an invalid input to the string to int function
            }
        }
        // string to int function will only throw invalid argument and out of range exceptions [see documentation] 
        catch (const std::invalid_argument& invalid_argument) { // if an invalid argument exception is thrown this block runs
            std::cout << "Invalid input! Please enter a menu option..." << std::endl;
        }
        catch (const std::out_of_range& out_of_range) { // if an out of range exception is thrown this block runs
            std::cout << "Invalid input! Please enter a menu option..." << std::endl;
        }
    } while (!valid_input);

    return input;
}
