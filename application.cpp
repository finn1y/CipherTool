#include "application.h"

application::application() {
    this->_runState = true; 
    // the current menu smart pointer is initialised with a pointer to the first menu (main menu)
    this->_currentMenu = menuPtr(new main_menu(this->_runState, this->_currentMenu));
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