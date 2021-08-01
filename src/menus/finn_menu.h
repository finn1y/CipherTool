#ifndef FINN_MENU_H
#define FINN_MENU_H

#include "menu.h"

// class for integrating all of my functions into the application
// standard menu class

class finn_menu: public menu {

    public:
        finn_menu(bool& runState, menuPtr& currentMenu);
        virtual ~finn_menu();

        void update();
        void print();        
    
    private:
        bool& _runState;
        menuPtr& _currentMenu;
};

#endif /* FINN_MENU_H */
