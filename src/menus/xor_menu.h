#ifndef XORMENU_H
#define XORMENU_H

#include "menu.h"
#include "../ciphers/xor.h"

// menu class for the xor cipher function of the application
// this menu class has several private functions specific to the xor cipher object as well as the standard menu functions

class xor_menu: public menu {

    public:
        xor_menu(bool& runState, menuPtr& currentMenu);
        ~xor_menu();

        void update();
        void print();

    private:
        bool& _runState;
        menuPtr& _currentMenu;

        // functions specific to the xor menu class 
        void xor_file(); // function for performing the xor cipher on a file input and writing the output to a file
        void xor_input(); // function for performing the xor cipher on a user input from the console
        void exit(); // function to exit the application based on a user input
        bool get_key(); // function to get a valid key from the user will return true if a valid key is gotten, false if not
        int get_file_size(std::string file_name); // function to get the size of a file

        XOR* _xor; // pointer to the xor class to create xor objects for performing the cipher
        std::vector<uint8_t> _bytes; // the vector of bytes that the cipher will be performed on
        bool _encryption; // value for encrypting or decrypting
};

#endif /* XORMENU */
