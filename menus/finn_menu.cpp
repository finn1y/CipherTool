#include "finn_menu.h"
#include "main_menu.h"
#include "xor_menu.h"
#include "aes_menu.h"

finn_menu::finn_menu(bool& runState, menuPtr& currentMenu): _runState(runState), _currentMenu(currentMenu) {}

finn_menu::~finn_menu() {}

void finn_menu::update() {
    this->print();

    int input = this->handle_int("\nEnter desired menu option: ", 3);

    switch (input) {
        case 1:
            // goes to xor menu for xor functions
            this->_currentMenu = menuPtr(new xor_menu(this->_runState, this->_currentMenu));
            break;
        
        case 2:
            // goes to aes menu for aes functions
            this->_currentMenu = menuPtr(new aes_menu(this->_runState, this->_currentMenu));
            break;

        case 3:
            // goes back to previous menu
            this->_currentMenu = menuPtr(new main_menu(this->_runState, this->_currentMenu));
            break;
    }
}

void finn_menu::print() {
    std::cout << "\n****** Mathematical Ciphers *****" << std::endl;
    std::cout << "|-------------------------------|" << std::endl;
    std::cout << "|   1: XOR Cipher\t\t|" << std::endl;
    std::cout << "|   2: Rjindael's Cipher (AES)\t|" << std::endl;
    std::cout << "|   3: Back\t\t\t|" << std::endl;
    std::cout << "|-------------------------------|" << std::endl;
}
