#include "main_menu.h"
#include "james_menu.h"

// string to int function documentation at https://docs.microsoft.com/en-us/cpp/standard-library/string-functions?view=msvc-160#stoi

main_menu::main_menu(bool& runState, menuPtr& currentMenu): _runState(runState), _currentMenu(currentMenu) {} 

main_menu::~main_menu() {}

void main_menu::update() { // anything that happens in the menu goes in this function 
    this->print();

    int input = this->handle_int("\nEnter a menu option: ", 4);

    switch (input) {
        case 1:
            std::cout << "finn menu" << std::endl;
            break;

        case 2:
            std::cout << "jj menu" << std::endl;
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
    /* temporary user interface for debugging */
    std::cout << "*********Encrpyter3000*********" << std::endl;
    std::cout << "\n*****Main Menu******" << std::endl;
    std::cout << "|-----------------------|" << std::endl;
    std::cout << "|   1: Finn stuff\t|" << std::endl;
    std::cout << "|   2: JJ stuff\t\t|" << std::endl;
    std::cout << "|   3: James stuff\t|" << std::endl;
    std::cout << "|   4: Exit\t\t|" << std::endl;
    std::cout << "|-----------------------|" << std::endl;
}
