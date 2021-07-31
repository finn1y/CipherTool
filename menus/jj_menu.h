#ifndef JJ_MENU_H
#define JJ_MENU_H

#include "menu.h"
#include "otp.h"


// class for integrating all of my functions into the application
// standard menu class

class jj_menu: public menu {

    public:
        jj_menu(bool& runState, menuPtr& currentMenu);
        virtual ~jj_menu();

        void update();
        void print();       

    
    private:
        bool& _runState;
        menuPtr& _currentMenu;

		OTP otp;
		std::string OTPDecode(std::string encodedString, std::string key);
		std::string OTPEncode(std::string decodedString, std::string key);
		std::string generateKey();
		std::string getFromFile(std::string fileName, bool isKey);
		std::string getFileName();
		void checkKeyLength(std::string key, std::string encodedString);
		void outputToFile(std::string output);
};

#endif /* JJ_MENU_H */
