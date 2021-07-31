#ifndef APPLICATION_H
#define APPLICATION_H

#include "main_menu.h"

class application {

    public:
        application();
        ~application();

        // function to run the 'main application' loop
        void run();

    private:
        bool _runState;
        menuPtr _currentMenu;
};

#endif /* APPLICATION_H */
