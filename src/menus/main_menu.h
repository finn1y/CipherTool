#ifndef MAINMENU_H
#define MAINMENU_H

#include "menu.h" 
#include "finn_menu.h"
#include "jj_menu.h"
#include "james_menu.h"

class main_menu: public menu {

    public:
        main_menu(bool& runState, menuPtr& currentMenu); 
        virtual ~main_menu();

        // pure virtual functions need to be declared and defined within the derived class
        void update();
        void print(); 

    private:
        // all menus take references to the applications run state and current menu allowing them to be changed from within this class
        bool& _runState; 
        menuPtr& _currentMenu;
};

#endif /* MAINMENU_H */
