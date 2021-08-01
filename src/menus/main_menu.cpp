#include "main_menu.h"
#include "finn_menu.h"
#include "jj_menu.h"
#include "james_menu.h"

// string to int function documentation at https://docs.microsoft.com/en-us/cpp/standard-library/string-functions?view=msvc-160#stoi

main_menu::main_menu(bool& runState, menuPtr& currentMenu): _runState(runState), _currentMenu(currentMenu) {} 

main_menu::~main_menu() {}

void main_menu::update() { // anything that happens in the menu goes in this function 
    this->print();

    int input = this->handle_int("\nEnter a menu option: ", 4);

    switch (input) {
        case 1:
            this->_currentMenu = menuPtr(new finn_menu(this->_runState, this->_currentMenu));
            break;

        case 2:
            this->_currentMenu = menuPtr(new jj_menu(this->_runState, this->_currentMenu));
            break;

        case 3:
            this->_currentMenu = menuPtr(new james_menu(this->_runState, this->_currentMenu));
            break;

        case 4:
            this->_runState = false; // exits the application by ending the 'run' loop
            break;
    }
}

void main_menu::print() { // prints the user interface to the command line
    std::cout << "\n*************** Main Menu ***************" << std::endl;
    std::cout << "|---------------------------------------|" << std::endl;
    std::cout << "|   1: Mathematical Ciphers (FMB)\t|" << std::endl;
    std::cout << "|   2: One Time Pad (JB)\t\t|" << std::endl;
    std::cout << "|   3: Substitution Ciphers (JNL)\t|" << std::endl;
    std::cout << "|   4: Exit\t\t\t\t|" << std::endl;
    std::cout << "|---------------------------------------|" << std::endl;
}
