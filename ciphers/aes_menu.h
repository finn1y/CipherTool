#ifndef AESMENU_H
#define AESMENU_H

#include "menu.h"
#include "aes.h"

// menu class for the aes cipher function of the application
// this menu class has several private functions specific to the aes cipher object as well as the standard menu functions

class aes_menu: public menu {

    public:
        aes_menu(bool& runState, menuPtr& currentMenu);
        ~aes_menu();

        void update();
        void print();

    private:
        bool& _runState;
        menuPtr& _currentMenu;

        // functions specific to the aes menu class 
        void aes_file(); // function for performing the aes cipher on a file input and writing the output to a file
        void aes_input(); // function for performing the aes cipher on a user input from the console
        void exit(); // function to exit the application based on a user input
        bool get_key(); // function to get a valid key from the user will return true if valid key gotten, false if not
        int get_file_size(std::string file_name); // function to count the number of lines in a file

        aes* _aes; // pointer to the aes class to create xor objects for performing the cipher
        std::vector<uint8_t> _bytes; // the vector of bytes that the cipher will be performed on
        bool _encryption; // value for encrypting or decrypting
};

#endif /* AESMENU_H */
