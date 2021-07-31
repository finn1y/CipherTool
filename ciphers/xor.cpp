#include "xor.h"

XOR::XOR(std::vector<uint8_t>& bytes): _bytes(bytes) {}

XOR::~XOR() {}

// xor encrpytion - bitwise xor against the key
void XOR::encrypt() {
    if (!this->_key.empty()) {
        // the key vector for the cipher must be the same size as the bytes vector
        // if it is not then a size of exception will be thrown  
        if (this->_key.size() < this->_bytes.size()) {
            // if the key vector is smaller than the bytes vector then the key will be appended to itself until they are the same size
            for (size_t i = 0; i < this->_bytes.size(); i++) {
                this->_key.push_back(this->_key[i]);
            }
        }

        for (size_t i = 0; i < this->_bytes.size(); i++) {
            this->_bytes[i] ^= this->_key[i];
        }
    }
    else {
        // if the user tries to encrypt or decrpyt without a key an exception is thrown
        throw std::invalid_argument("No key input!");
    }    
}

// generates a random key using random device from c++ standard library
void XOR::gen_key() {
    this->_key.clear();
    // generates a random byte (uint8_t) and appends it to the key vector until the key is the same size as the bytes vector
    while (this->_key.size() < this->_bytes.size()) {
        uint8_t byte = this->gen();
        // only alpha numeric ascii characters will be used (can be printed to console)
        if (byte > 32 && byte < 126) {
            this->_key.push_back(byte);
        }
    }

    // creates a string out of the key vector for printing to console
    std::string key;
    for (size_t i = 0; i < this->_key.size(); i++) {
        key.push_back(this->_key[i]);
    }

    // prints key to console for user to note for decrpytion
    std::cout << "\nKey: " << key << std::endl;
}

// user defined key set to key member
void XOR::set_key(std::string key) {
    if (!key.empty()) {
        // if key is valid (not an empty string) then set key to private member
        this->_key.clear();
        this->_key.resize(key.size());

        for (size_t i = 0; i < key.size(); i++) {
            this->_key[i] = key[i];
        }
    }
    else {
        // if key is not valid (is an empty string) out of range exception thrown
        throw std::out_of_range("No key input!"); 
    }
}
