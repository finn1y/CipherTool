#ifndef JAMES_MENU_H
#define JAMES_MENU_H

#include "menu.h"
#include <vector>

// class for integrating all of my functions into the application
// standard menu class

class james_menu: public menu {

    public:
        james_menu(bool& runState, menuPtr& currentMenu);
        virtual ~james_menu();

        void update();
        void print();
        void caesars();
        void caesarFile_encrypt(std::string text);
        void caesarFile_decrypt(std::string text);
        void caesarInput_encrypt(std::string text);
        void caesarInput_decrypt(std::string text);
        void ROT13();
        void ROT13_input(std::string text);
        void ROT13_file(std::string filename); 
        void vigenere_Input(std::string, std::string alphabet);
        void wordtonum(std::string keyword);
        void vigenere();
        std::string makekey(std::string keyIn, std::string textIn);
        std::string vigenere_translate(std::string text, std::string alphabet);
    
    private:
        bool& _runState;
        menuPtr& _currentMenu;

        std::string shift_translate(std::string text, int key, std::string alphabet);
        std::string text;
        std::string user_in;
        std::vector <int> dynkey; 
};

#endif /* JAMES_MENU_H */
