#ifndef CODER_H
#define CODER_H

#include <iomanip>

#include "xor.h"
#include "aes.h"
#include "otp.h"

// enum of available ciphers 
enum Ciphers {
    Xor,
    Aes,
    Otp,
    Caesars,
    Rot13,
    Vigenere
};

// class grouping ciphers together into single object

class Coder {

    public:
        Coder();
        Coder(Ciphers cipher, std::string key);
        ~Coder();

        //functions to set cipher and key private members
        void set_cipher(Ciphers cipher);
        void set_key(std::string key);

        //functions to encrypt or decrypt based on cipher and key members
        std::string encrypt(std::string plaintext);
        std::string decrypt(std::string ciphertext);

    private:
        //variables to hold the current cipher and associated key
        Ciphers _cipher;
        std::string _key;

        //pointers to each of the cipher classes
        XOR* _xor;
        aes* _aes;
        OTP* _otp;

        //functions to perform encryption or decryption for ciphers
        std::string xor_cipher(int en_de, std::string input);
        std::string aes_cipher(int en_de, std::string input);
        std::string otp_cipher(int en_de, std::string input);
        std::string caesars_cipher(int en_de, std::string input);
        std::string rot13_cipher(int en_de, std::string input);
        std::string vigenere_cipher(int en_de, std::string input);
        std::string makekey(std::string input);
        std::vector<int> vigenere_key(std::string keyword);

};

#endif /* CODER_H */
