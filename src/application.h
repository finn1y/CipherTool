#ifndef APPLICATION_H
#define APPLICATION_H

#include "menus/main_menu.h"

enum Menus {
    main_m,
    xor_m,
    aes_m,
    jj_m,
    james_m
};

class application {

    public:
        application(Menus start_menu);
        ~application();

        // function to run the 'main application' loop
        void run();

    private:
        bool _runState;
        menuPtr _currentMenu;
};

#endif /* APPLICATION_H */
