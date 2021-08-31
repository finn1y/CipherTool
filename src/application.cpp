#include "application.h"
#include "menus/xor_menu.h"
#include "menus/aes_menu.h"

application::application(Menus start_menu) {
    this->_runState = true; 
    
    switch (start_menu) {
        case main_m:
            // the current menu smart pointer is initialised with a pointer to the first menu (main menu)
            this->_currentMenu = menuPtr(new main_menu(this->_runState, this->_currentMenu));
            break;

        case xor_m:
            this->_currentMenu = menuPtr(new xor_menu(this->_runState, this->_currentMenu));
            break;

        case aes_m:
            this->_currentMenu = menuPtr(new aes_menu(this->_runState, this->_currentMenu));
            break;

        case jj_m:
            this->_currentMenu = menuPtr(new jj_menu(this->_runState, this->_currentMenu));
            break;

        case james_m:
            this->_currentMenu = menuPtr(new james_menu(this->_runState, this->_currentMenu));
            break;
    }
}

application::~application() {
    // clean-up releases the smart pointer from the current menu pointer
    this->_currentMenu.release();
}

void application::run() {
    while (this->_runState) {
        this->_currentMenu->update(); // calls the update function for the current menu
    }
}
