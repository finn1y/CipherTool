#include "application.h"

std::string cipher = "Null";
std::string file = "";
std::string key = "";
bool gui = false;
const std::string ciphers[6] = {"XOR", "AES", "OTP", "Caesars", "ROT13", "Vigenere"};

void help();
Menus start_menu(std::string cipher);

int main(int argc, char** argv) {
    for (int i = 0; i < argc; i++) {
        std::string arg = std::string(argv[i]);

        if (arg == "--help" || arg == "-h") {
            help();
            return 0;
        }
        else {
            if (arg.substr(0,9) == "--cipher=") {
                cipher = arg.substr(9, std::string::npos); 
            }
            else if (arg == "--cipher" || arg == "-c") {
                i++;
                cipher = std::string(argv[i]);

                bool cipher_found = false;
                for (int i = 0; i < 6; i++) {
                    if (cipher == ciphers[i] || cipher == "Null") {
                        cipher_found = true;
                    }
                }

                if (cipher_found == false) {
                    std::cout << "Cipher not available in tool" << std::endl;
                    help();
                    return 0;
                }
            }
            else if (arg.substr(0,5) == "file=") {
                file = arg.substr(5, std::string::npos);
            }
            else if (arg == "--file" || arg == "-f") {
                i++;
                file = std::string(argv[i]);
            }
            else if (arg.substr(0,4) == "key=") {
                key = arg.substr(4, std::string::npos);
            }
            else if (arg == "--key" || arg == "-k") {
                i++;
                key = std::string(argv[i]);
            }
            else if (arg == "--gui" || arg == "-g") {
                gui = true;
            }
        }
    }

    if ( gui == true ) {
        application app(start_menu(cipher));
        app.run(); 
    }
    else {
        std::cout << "cipher: " << cipher << "\nfile: " << file << "\nkey: " << key << std::endl;
    }

    return 0; 
}

void help() {
    std::cout << "Usage: cipher_tool [OPTION]" << std::endl;
}

Menus start_menu(std::string cipher) {
    Menus s_menu = main_m;

    for (int i = 0; i < 6; i++) {
        if (cipher == ciphers[i]) {
            //if ROT13 or Vigenere then go to james menu
            if ((i + 1) > 4) {
                s_menu = james_m;
            }
            //else menu value will be index of cipher + 1
            else {
                s_menu = static_cast<Menus>(i + 1);
            }
        }
    }

    return s_menu;
}

