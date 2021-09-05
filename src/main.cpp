#include "application.h"
#include "ciphers/coder.h"

const std::string ciphers[6] = {"XOR", "AES", "OTP", "Caesars", "ROT13", "Vigenere"};

void help();
Menus start_menu(std::string cipher);
std::string read_file(std::string file_name);
void write_file(std::string file_name, std::string data);
int get_file_size(std::string file_name);

int main(int argc, char** argv) {
    std::string cipher = "Null";
    Ciphers coder_cipher = Xor;
    std::string in_file = "";
    std::string out_file = "";
    std::string key = "";
    bool gui = false;
    bool en = true;

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
                if (cipher != "Null") {
                    for (int i = 0; i < 6; i++) {
                        if (cipher == ciphers[i]) {
                            cipher_found = true;
                            coder_cipher = static_cast<Ciphers>(i);
                        }
                    }
                }

                if (cipher_found == false) {
                    std::cout << "Cipher not available in tool" << std::endl;
                    help();

                    return 0;
                }
            }
            else if (arg.substr(0,5) == "--input-file=") {
                in_file = arg.substr(5, std::string::npos);
            }
            else if (arg == "--input-file" || arg == "-i") {
                i++;
                in_file = std::string(argv[i]);
            }
            else if (arg.substr(0,5) == "--output-file=") {
                out_file = arg.substr(5, std::string::npos);
            }
            else if (arg == "--output-file" || arg == "-o") {
                i++;
                out_file = std::string(argv[i]);
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
            else if (arg == "--encrypt" || arg == "-e") {
                en = true;
            }
            else if (arg == "--decrypt" || arg == "-d") {
                en = false;
            }
        }
    }

    if ( gui == true ) {
        application app(start_menu(cipher));
        app.run(); 
    }
    else {
        if (in_file.empty()) {
            std::cout << "Error: missing required argument IN_FILE" << std::endl;
            help();

            return 1;
        }

        if (out_file.empty()) {
            out_file = in_file;
        }

        Coder coder(coder_cipher, key);

        if (en) {
            std::string ciphertext = coder.encrypt(read_file(in_file));
            write_file(out_file, ciphertext);
        }
        
        if (!en) {
            std::string plaintext = coder.decrypt(read_file(in_file));
            write_file(out_file, plaintext);
        }
    }

    return 0; 
}

void help() {
    std::cout << "Usage: cipher_tool [OPTION] --gui/--input-file=IN_FILE" << std::endl;
    std::cout << "Options:" <<std::endl;
    std::cout << "  -c, --cipher=CIPHER\t\tcipher to encrypt/decrypt with defaults to XOR" << std::endl;
    std::cout << "  -d, --decrypt\t\t\ttool will decrypt the input" << std::endl;
    std::cout << "  -e, --encrypt\t\t\ttool will encrypy the input default" << std::endl;
    std::cout << "  -g, --gui\t\t\tuse the tool with gui" << std::endl;
    std::cout << "  -h, --help\t\t\tshows this message" << std::endl;
    std::cout << "  -i, --input-file=IN_FILE\tfile to read input data from required when not using gui" << std::endl;
    std::cout << "  -k, --key=KEY\t\t\tkey used by some ciphers defualts to random key" << std::endl;
    std::cout << "\t\t\t\tsee Ciphers for specific info on keys required for each cipher" << std::endl;
    std::cout << "  -o, --output-file=OUT_FILE\tfile to write output data to defaults to same as IN_FILE" << std::endl;
    std::cout << "\nCiphers:" << std::endl;
    std::cout << "  XOR\t\tKEY of any length as a string" << std::endl;
    std::cout << "  AES\t\tKEY of any length as a string" << std::endl;
    std::cout << "  OTP\t\tKEY of length equal to or greater than data as a string" << std::endl;
    std::cout << "  Caesars\tKEY as an integer" << std::endl;
    std::cout << "  ROT13\t\tno KEY taken" << std::endl;
    std::cout << "  Vigenere\tKEY of any length as a string" << std::endl;
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

std::string read_file(std::string file_name) {
    std::ifstream in_file(file_name);
    std::stringstream data;

    if (in_file.is_open()) {
        // get the size of the file
        int size = get_file_size(file_name);
        int counter = 0;
        while (!in_file.eof()) {
            std::string line{};
            // each line is read individually to get any formatting, e.g. whitespace and tabs
            std::getline(in_file, line);
            data << line;
            counter++; // a counter is incremented
            // the new line character should not be added on the final line
            if (counter != size) {
                // the new line character is excluded by the getline function so it must be added manually
                data << std::endl;
            }
        }
    }
    in_file.close();

    return data.str();
}

void write_file(std::string file_name, std::string data) {
    std::ofstream out_file(file_name);

    if (out_file.is_open()) {
        out_file << data;
    } 

    out_file.close();
}

// function to count the number of lines in a file
int get_file_size(std::string file_name) {
    int count = 0;
    std::ifstream file(file_name);

    if (file.is_open()) {
        std::string line{};
        // read the lines of the file until the end of the file
        while (!file.eof()) {
            std::getline(file, line);
            // every time a line is read the counter increments
            count++;
        }
        // once the end of the file is reached close the file
        file.close();
    }

    return count;
}

