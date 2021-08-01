#ifndef MENU_H
#define MENU_H

#include <iostream> 
#include <memory>
#include <string>
#include <fstream>
#include <iomanip>
#include <fstream>

// abstract base class uses pure virtual functions which do not need to be declared
// using pure virtual functions means any derived menu class can define the functions as needed by that specific class
// abstract base menu class used to derive all other menu classes

class menu {
        
    public:
        // virtual destructor needed for re-definition of destructor by unique pointer
        virtual ~menu();

        std::string handle_input(std::string str); 
        int handle_int(std::string str, int n);

        // setting a virtual function = 0 makes it a pure virtual function and needs no definition
        virtual void update() = 0;
        virtual void print() = 0;
};

// define type of a smart pointer to the abstract menu class
// used in application rather than raw pointer for dynamic movement of the pointer between the different derived menu classes
// in this way the application can call the update function on whichever derived menu class the smart pointer is currently pointing to
typedef std::unique_ptr<menu> menuPtr; 

#endif /* MENU_H */
