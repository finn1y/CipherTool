#include <iostream>
#include <iomanip>

#include "aes.h"

int main() {
    // input = 3243f6a8885a308d313198a2e0370734
    std::vector<uint8_t> bytes = {0x32,0x43,0xf6,0xa8,0x88,0x5a,0x30,0x8d,0x31,0x31,0x98,0xa2,0xe0,0x37,0x07,0x34};

    aes aes(bytes);

    // key = 2b7e151628aed2a6abf7158809cf4f3c
//    ustring key = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c}; 
    aes.set_key("hello world hello worl");

    aes.encrypt();

    std::cout << "encryption:" << std::endl;
    for (size_t i = 0; i < bytes.size(); i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << int(bytes[i]) << ", ";
    }
    std::cout << std::endl;

    aes.decrypt();

    std::cout << "decryption:" << std::endl;
    for (size_t i = 0; i < bytes.size(); i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << int(bytes[i]) << ", ";
    }
    std::cout << std::endl;

    return 0;
}
