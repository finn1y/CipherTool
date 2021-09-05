#ifndef CODER_H
#define CODER_H

#include <iomanip>

#include "xor.h"
#include "aes.h"
#include "otp.h"

enum Ciphers {
    Xor,
    Aes,
    Otp,
    Caesars,
    Rot13,
    Vigenere
};

class Coder {

    public:
        Coder();
        Coder(Ciphers cipher, std::string key);
        ~Coder();

        void set_cipher(Ciphers cipher);
        void set_key(std::string key);

        std::string encrypt(std::string plaintext);
        std::string decrypt(std::string ciphertext);

    private:
        Ciphers _cipher;
        std::string _key;

        XOR* _xor;
        aes* _aes;
        OTP* _otp;

        std::string xor_cipher(int en_de, std::string input);
        std::string aes_cipher(int en_de, std::string input);
        std::string otp_cipher(int en_de, std::string input);
        std::string caesars_cipher(int en_de, std::string input);
        std::string rot13_cipher(int en_de, std::string input);
        std::string vigenere_cipher(int en_de, std::string input);

};

#endif /* CODER_H */
