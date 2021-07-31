#ifndef XOR_H
#define XOR_H

#include <iostream>
#include <vector>
#include <cstdint> // included for uint8_t type
#include <random> // included for random_device object 

// documentation for random device at: https://docs.microsoft.com/en-us/cpp/standard-library/random-device-class?view=msvc-160
// the random device must be used for random number generation in cryptography as other engines and distributions have too low entropy
// i.e. other standard pseudo-random number generators do not have enough "randomness" in their generation 

// class for encrpyting and decrypting a vector of bytes using the xor cipher

class XOR {

    public:
        XOR(std::vector<uint8_t>& bytes);
        ~XOR();

        // function to encrypt or decrpyt the current bytes vector - xor uses the same method for encryption and decryption
        void encrypt();

        // functions to generate a random key or to set a user defined key
        void gen_key();
        void set_key(std::string key);

    private:
        std::vector<uint8_t>& _bytes; // a reference to the bytes vector that the xor encryption/decryption occurs to
        std::vector<uint8_t> _key; // key member for encrypting/decrypting against
        std::random_device gen; // random number generator from the random c++ standard library
};

#endif /* XOR_H */
