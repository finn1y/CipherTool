#include "aes.h"
#include <iomanip>

aes::aes(std::vector<uint8_t>& bytes): _bytes(bytes) {
    // a temporary vector to store the 16 byte blocks
    std::vector<uint8_t> temp_byte_vector(size_t(16));
    
    // before the cipher can be performed the input bytes must be padded to block length if required
    this->pcks7_pad(this->_bytes);
    // aes-128 works of 128 bit (16 byte) blocks of data
    // the input byte vector must be copied and transformed into an array of 16 byte blocks
    for (size_t i = 0; i < this->_bytes.size(); i += 16) {
        for (int j = 0; j < 16; j++) {
            // the input bytes vector is looped through to get each 16 byte block stored in the temporary vector
            temp_byte_vector[j] = this->_bytes[i + j];
        }
        // each block is then appended to the blocks vector
        this->_blocks.push_back(temp_byte_vector);
    }
}

aes::~aes() {}

// function for encrypting the bytes vector (at this point held in blocks and written to bytes after encryption)
void aes::encrypt() {
    if (!this->_roundKeys.empty()) {
        // if a key has been inputted and expanded the cipher will be performed
        // initial round key addition 
        this->addRoundKey(0);

        // the main 9 rounds consist of each main cipher operation done in order
        for (int i = 1; i < 10; i++) {
            this->subBytes();
            this->shiftRows();
            this->mixColumns();
            this->addRoundKey(i);
        }

        // the final round excludes the mix columns operation
        this->subBytes();
        this->shiftRows();
        this->addRoundKey(10);
        // once the encrpytion is complete the encrypted blocks must be written to the bytes vector reference
        for (size_t i = 0; i < this->_blocks.size(); i++) {
            for (int j = 0; j < 16; j++) {
                this->_bytes[(i * 16) + j] = this->_blocks[i][j];
            }
        }
    }
    else {
        // if no key has been inputted but the user attempts to encrypt an exception will be thrown
        throw std::invalid_argument("No key input!");
    }
}

// function for decrypting the bytes vector (at this point held in blocks and written to bytes after decryption)
void aes::decrypt() {
    if (!this->_roundKeys.empty()) {
        // if a key has been inputted and expanded the inverse cipher will be performed
        // the final round of the forward cipher is performed first in reverse order
        this->addRoundKey(10);
        this->inv_shiftRows();
        this->inv_subBytes();

        // the next 9 rounds are perfomed in reverse order
        for (int i = 9; i > 0; i--) { // the iterator starts at 9 and decrements to select the round keys in reverse order
            this->addRoundKey(i);
            this->inv_mixColumns();
            this->inv_shiftRows();
            this->inv_subBytes();
        } 

        // final round key addition
        this->addRoundKey(0);

        // once the decryption is complete the decrypted blocks must be written to the bytes vector reference
        for (size_t i = 0; i < this->_blocks.size(); i++) {
            for (int j = 0; j < 16; j++) {
                this->_bytes[(i * 16) + j] = this->_blocks[i][j];
            }
        }
        // unpad the decrypted plaintext if necessary
        this->pcks7_unpad(this->_bytes);
    }
    else {
        // if no key had been inputted but the user attempts to decrypt an exception will be thrown
        throw std::invalid_argument("No key input!");
    }
}

// user defined key set to key member
void aes::set_key(std::string key) {
    if (!key.empty()) {
        // key size limit for aes is 256 bits 
        if (key.size() > 32) {
            // if key is greater than 256 bits (32 bytes) then an exception is thrown
            throw std::invalid_argument("Key too large!");
        }

        // if key is valid (not an empty string) then set key member as input
        this->_key.clear();
        this->_key.resize(key.size());

        for (size_t i = 0; i < key.size(); i++) {
            this->_key[i] = key[i];
        }

        // the key must have a minimum 128 bit length
        // if the key is shorter than this it will be padded to a multiple of 128 bit length
        this->pcks7_pad(this->_key);

        // the key is expanded to produce the round keys used by the cipher
        this->keyExpansion();
    }
    else {
        // if key is not valid (empty string) out of range exception is thrown
        throw std::out_of_range("No key input!");
    }
}

// generate a random key using random device from c++ standard library
void aes::gen_key() {
    this->_key.clear();
    // generate a random byte (uint8_t) and appends it to the key vector until a 16 byte key has been produced
    while (this->_key.size() < 16) {
        uint8_t byte = this->gen();
        // only alphanumeric ascii characters will be used (can be printed to the command line)
        if (byte > 32 && byte < 126) {
            this->_key.push_back(byte);
        }
    }

    // creates a string out of the key vector for printing to console
    std::string key;
    for (size_t i = 0; i < this->_key.size(); i++) {
        key.push_back(this->_key[i]);
    }

    // prints the key to console for user to note for decryption
    std::cout << "\nKey: " << key << std::endl;

    // the key is expanded to prodice the round keys used by the cipher
    this->keyExpansion();
}

/*************************************************************************************************************************************
* Title: Key Expansion
* Authors: J.Daemen, V.Rijmen
* Date: 1999
* Code version: 2
* Available: https://www.cs.miami.edu/home/burt/learning/Csc68.012/rjindael/rjindael_doc_V2.pdf
* Pages: 14-15
*************************************************************************************************************************************/

// this function has been derived from the above source
// key expansion function as defined in the cipher
void aes::keyExpansion() {
    // generate the round constants to be XORed within the key expansion
    // only the first byte of each word is XORed with the round constant as defined in the cipher
    std::vector<std::vector<uint8_t>> rcon = {{1, 0, 0, 0}}; 
    for (int i = 0; i < 10; i++) {
        rcon.push_back({uint8_t(this->gf256mul(rcon[i][0], 2)), 0, 0, 0});
    }
    std::cout << std::endl;
    
    // define a vraible as the size of the input key in bytes for easy use in functions
    int bytes_in_key = int(this->_key.size());

    // the expanded key vector is used to temporarily store the full expanded key while the key expansion is calculated
    // the first n bytes of the expanded key are the bytes of the input key
    std::vector<uint8_t> expandedKey = this->_key;
    // within this implementation the numbers are for bytes in the key rather than words as defined in the cipher
    // this implementation is also for aes 128 bit block length and 10 rounds only therefore some variables are replaced with constants
    if (bytes_in_key <= 24) {
        // for aes 128 bit block length and 10 rounds 176 bytes of key are needed
        for (int i = bytes_in_key; i < 176; i += 4) {
            // a temporary vector of 4 bytes (a word) is created from the previous word
            std::vector<uint8_t> temp_word = {expandedKey[i - 4], expandedKey[i - 3], expandedKey[i - 2], expandedKey[i - 1]};
            if (i % bytes_in_key == 0) { // every 16 bytes a calculation is performed
                // the word is byte-wise circular left shifted
                temp_word = this->rotByte(temp_word);
                for (int j = 0; j < 4; j++) {
                    // the word is then mapped on to its Rjindael's s-box equivalent and XORed with the relevant round key
                    temp_word[j] = this->sBox(temp_word[j]) ^ rcon[int(i / bytes_in_key) - 1][j];
                }
            }
            // the temp word is XORed with the initial word and written to the vector 
            for (int j = 0; j < 4; j++) {
                expandedKey.push_back(expandedKey[i + j - (bytes_in_key)] ^ temp_word[j]);
            }
        } 
    }
    else if (bytes_in_key > 24) {
        // for aes 128 bit block length and 10 rounds 176 bytes of key are needed
        for (int i = bytes_in_key; i < 176; i += 4) {
            // a temporary vector if 4 bytes (a word) is created from the previous word
            std::vector<uint8_t> temp_word = {expandedKey[i - 4], expandedKey[i - 3], expandedKey[i - 2], expandedKey[i-1]};
            if (i % bytes_in_key == 0) { // every 16 bytes a calculation is performed
                // the word is byte-wise circular left shifted
                temp_word = this->rotByte(temp_word);
                for (int j = 0; j < 4; j++) {
                    // the word is then mapped on to Rjindael's s-box and XORed with the relevant round key
                    temp_word[j] = this->sBox(temp_word[j]) ^ rcon[int(i / bytes_in_key) - 1][j];
                }
            }
            else if (i % bytes_in_key == 4) { // the word after the word that the calculation is performed on is mapped on to Rjindael's s-box
                for (int j = 0; j < 4; j++) {
                    temp_word[j] = this->sBox(temp_word[j]);
                }
            }
            // the temp word is XORed with the initial word and written to the vector
            for (int j = 0; j < 4; j++) {
                expandedKey.push_back(expandedKey[i + j - (bytes_in_key)] ^ temp_word[j]);
            }
        }
    }
    // the expanded key is then written to the round keys vector
    // the round keys vector contains 16 byte blocks of the expanded key as a vector
    this->_roundKeys.resize(11); // 11 round keys are required for 10 rounds of the cipher
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 16; j++) {
            this->_roundKeys[i].push_back(expandedKey[(i * 16) + j]);
        }
    }
}

// adding the relevant round key to the block using XOR which is equivalent to addition in GF(256)
void aes::addRoundKey(int round) {
    for (size_t i = 0; i < this->_blocks.size(); i++) {
        for (size_t j = 0; j < 16; j++) {
            this->_blocks[i][j] ^= this->_roundKeys[round][j];
        }
    }
}

// substitute each individual byte for its s-box mapping
void aes::subBytes() {
    // loop through each block
    for (size_t i = 0; i < this->_blocks.size(); i++) {
        // loop through each byte within the block
        for (size_t j = 0; j < 16; j++) {
            this->_blocks[i][j] = this->sBox(this->_blocks[i][j]);
        }
    }
}

// based of the state defined in the cipher each row of bytes is left circular shifted by an increasing amount
void aes::shiftRows() {
    for (size_t i = 0; i < this->_blocks.size(); i++) {
        // the first row is no shifted, the second is shifted by 1, the third by 2 and the fourth by 3
        // due to the nature of the defined state within the vector each row represents a column in the state
        // therefore the transformation defined in the cipher is mirrored through the negative diagonal 
        this->_blocks[i] = {this->_blocks[i][0], this->_blocks[i][5], this->_blocks[i][10], this->_blocks[i][15],
                            this->_blocks[i][4], this->_blocks[i][9], this->_blocks[i][14], this->_blocks[i][3],
                            this->_blocks[i][8], this->_blocks[i][13], this->_blocks[i][2], this->_blocks[i][7],
                            this->_blocks[i][12], this->_blocks[i][1], this->_blocks[i][6], this->_blocks[i][11]};
    }
}

// based on the state defined in the cipher each column is transformed using the matrix transformation defined in the cipher
void aes::mixColumns() {
    // loop through each 16 byte block
    for (size_t i = 0; i < this->_blocks.size(); i++) {
        // loop through each 4 byte column in each block
        for (size_t j = 0; j < 16; j += 4) {    
            // create a temporary array of bytes for easy matrix multiplication
            uint8_t column[4] = {this->_blocks[i][j], this->_blocks[i][j + 1], this->_blocks[i][j + 2], this->_blocks[i][j + 3]};
            // using the matrix multiplication defined by Rjindael in the proposal as explained in the wiki
            // perform the mix column transformation on each set of 4 bytes:
            // [b0] = [02 03 01 01][a0]
            // [b1]   [01 02 03 01][a1]
            // [b2]   [01 01 02 03][a2]
            // [b3]   [03 01 01 02][a3]
            // in GF(256) XOR is the equivalent of addition
            this->_blocks[i][j] = this->gf256mul(column[0], 2) ^ this->gf256mul(column[1], 3) ^ column[2] ^ column[3];
            this->_blocks[i][j + 1] = column[0] ^ this->gf256mul(column[1], 2) ^ this->gf256mul(column[2], 3) ^ column[3];
            this->_blocks[i][j + 2] = column[0] ^ column[1] ^ this->gf256mul(column[2], 2) ^ this->gf256mul(column[3], 3);
            this->_blocks[i][j + 3] = this->gf256mul(column[0], 3) ^ column[1] ^ column[2] ^ this->gf256mul(column[3], 2);
        }
    }
}

// substitute each individual byte for its inverse s-box mapping
void aes::inv_subBytes() {
    // loop through each block
    for (size_t i = 0; i < this->_blocks.size(); i++) {
        // loop through each byte within the block
        for (size_t j = 0; j < 16; j++) {
            this->_blocks[i][j] = this->inv_sBox(this->_blocks[i][j]);
        }
    }
}

// based on the state defined in the cipher each row of bytes is right circular shifter by an increasing amount
void aes::inv_shiftRows() {
    // loop through each block
    for (size_t i = 0; i < this->_blocks.size(); i++) {
        // the first row is not shifted, the second is shifted by 1, the third by 2 and the fourth by 3
        // as described in the shift rows section
        this->_blocks[i] = {this->_blocks[i][0], this->_blocks[i][13], this->_blocks[i][10], this->_blocks[i][7],
                            this->_blocks[i][4], this->_blocks[i][1], this->_blocks[i][14], this->_blocks[i][11],
                            this->_blocks[i][8], this->_blocks[i][5], this->_blocks[i][2], this->_blocks[i][15],
                            this->_blocks[i][12], this->_blocks[i][9], this->_blocks[i][6], this->_blocks[i][3]};
    }
}

// based on the state defined in the cipher transform each column using the matrix tranformation defined in the cipher
void aes::inv_mixColumns() {
    // loop through each block
    for (size_t i = 0; i < this->_blocks.size(); i++) {
        // loop through each 4 byte column within each block
        for (size_t j = 0; j < 16; j += 4) {
            // tempoary array of bytes for easy matrix multiplication
            uint8_t column[4] = {this->_blocks[i][j], this->_blocks[i][j + 1], this->_blocks[i][j + 2], this->_blocks[i][j + 3]};
            // using the matrix multiplication defined by Rjindael in the proposal as explained in the wiki
            // perform the inverse of the mix columns on each set of 4 bytes
            // [a0] = [14 11 13 09][b0]
            // [a1]   [09 14 11 13][b1]
            // [a2]   [13 09 14 11][b2]
            // [a3]   [11 13 09 14][b3]
            // in GF(256) XOR is the equivalent of addition
            this->_blocks[i][j] = this->gf256mul(column[0], 14) ^ this->gf256mul(column[1], 11) ^ this->gf256mul(column[2], 13) ^ this->gf256mul(column[3], 9);
            this->_blocks[i][j + 1] = this->gf256mul(column[0], 9) ^ this->gf256mul(column[1], 14) ^ this->gf256mul(column[2], 11) ^ this->gf256mul(column[3], 13);
            this->_blocks[i][j + 2] = this->gf256mul(column[0], 13) ^ this->gf256mul(column[1], 9) ^ this->gf256mul(column[2], 14) ^ this->gf256mul(column[3], 11);
            this->_blocks[i][j + 3] = this->gf256mul(column[0], 11) ^ this->gf256mul(column[1], 13) ^ this->gf256mul(column[2], 9) ^ this->gf256mul(column[3], 14);
        } 
    }
}

 
// calculate the s-box substitution defined in the cipher
uint8_t aes::sBox(uint8_t byte) {
    // take the reciprocal of the byte
    int recip = this->gf256rcp(byte);
    int output = recip;

    // perform the affine transformation on the reciprocal
    for (int i = 0; i < 4; i++) {
        // affine transformation is equivalent to several left bitwise circular shifts of the byte:
        // [y0] = [1 0 0 0 1 1 1 1][x0] + [1]
        // [y1]   [1 1 0 0 0 1 1 1][x1]   [1]
        // [y2]   [1 1 1 0 0 0 1 1][x2]   [0]
        // [y3]   [1 1 1 1 0 0 0 1][x3]   [0]
        // [y4]   [1 1 1 1 1 0 0 0][x4]   [0]
        // [y5]   [0 1 1 1 1 1 0 0][x5]   [1]
        // [y6]   [0 0 1 1 1 1 1 0][x6]   [1]
        // [y7]   [0 0 0 1 1 1 1 1][x7]   [0]
        recip = this->left_circular_shift(recip);
        // these shifts are XORed against each other
        output ^= recip;
    }
    // final XOR against 0x63 as defined in the cipher
    return output ^= 0x63; // XOR is equivalent to addition in GF(256)
}

// calculate the inverse of the s-box substitution defined in the cipher
uint8_t aes::inv_sBox(uint8_t byte) {
    // an initial left circular shift is done to intialise the output
    uint8_t output = this->left_circular_shift(byte);
    // perform the inverse affine transformation
    for (int i = 0; i < 6; i++) {
        // inverse affine transformation is equivalent to serveral left bitwise circular shifts of the byte
        byte = this->left_circular_shift(byte);
        if (i == 2 || i == 5) { 
            output ^= byte; // only shifts of 1, 3 and 6 bits are XORed in the inverse transformation
        } 
    }    
    // final XOR against 0x05 as defined in the cipher
    output ^= 0x05;
    // the reciprocal is then raken to give the final output
    return this->gf256rcp(output);
}

/*****************************************************************************************************************************************
* Title: Galois Field Multiplication
* Author: S. Treholme
* Date: 2005
* Avaailable: https://www.samiam.org/galois.html
*****************************************************************************************************************************************/

// this function has been taken from the above source
// mathematical evidence and proof of this method for calculating finite field multiplication and division can be found in the wiki
// calculate the product of two ints in Galois finite field GF(256)
int aes::gf256mul(int a, int b) {
    // the product of anything and 0 is always 0
    if (a == 0 || b == 0) {
        return 0;
    }

    // the sum of the logs of the variables is the same as the log of the product of the variables
    // i.e. log(ab) = log(a) + log(b)
    int product = this->_logTable[a] + this->_logTable[b];
    product %= 255; // sum modulo 255 to reduce it within the finite field
    return this->_exponentTable[product]; // the final product is the anti-log (exponent of base 0x03)
}

// this function has been derived from the above source 
// calculate the multiplicative inverse (reciprocal) of an int in Galois finite field GF(256)
int aes::gf256rcp(int a) {
    // the multiplicative inverse of 0 in GF(256) is 0
    if (a == 0) {
        return 0;
    }
    
    // the multiplicative inverse is 1 divided by the value (the reciprocal) 
    // the subtraction of logs is the same as the log of a divided by b
    // i.e. log(a/b) = log(a) - log(b)
    int quotient = this->_logTable[1] - this->_logTable[a];
    quotient %= 255; // modulo 255 to reduce it to within the finite field
    return this->_exponentTable[quotient]; // the final product is the anti-log (exponent of base 0x03)
}

// left shift a byte set the lsb to be the msb and remove the msb 
uint8_t aes::left_circular_shift(uint8_t byte) {
    // set the msb 
    uint8_t msb = byte & 0x80;
    byte <<= 1; // left shift of a uint8_t type will remove the msb
        
    if (msb == 0x80) {
        return byte | 0x01; // bitwise OR byte with 1 to set the lsb to 1 if the msb was 1 
    }

    return byte; // if msb is 0 no bitwise OR needed
}

// left circular shift a 4 byte word bytewise
std::vector<uint8_t> aes::rotByte(std::vector<uint8_t> bytes) {
    return {bytes[1], bytes[2], bytes[3], bytes[0]};
}

// pads a vector of bytes to the required block length using pcks#7 padding
void aes::pcks7_pad(std::vector<uint8_t>& bytes) {
    // padding is only required if the vector of bytes is not to block length
    if (bytes.size() % 16 != 0) {
        // in pcks#7 padding the appended bytes equal the number of required bytes to pad to length
        // in aes 128 bits the block length is 16 bytes
        int pad_len = 16 - (bytes.size() % 16); // the padding length is first calculated
        
        // the padding is then appended to the vector 
        for (int i = 0; i < pad_len; i++) {
            bytes.push_back(pad_len);
        }
    }
}

// unpads a vector of bytes padded with pcks#7 padding
void aes::pcks7_unpad(std::vector<uint8_t>& bytes) {
    // in pcks#7 padding the length of the padding is equal to the appended bytes
    // the last byte of the array can be accessed with the index of the size of the array -1
    int pad_len = bytes.at(bytes.size() - 1);
    int counter = 0;

    // for valid padding the last n bytes must be equal to n where n is the pad length
    // the last n bytes are loop through starting with the last bytes
    for (size_t i = (bytes.size() - 1); i > ((bytes.size() - 1) - pad_len); i--) {
        // if the bytes is equal to the pad length a counter is incremented
        if (bytes[i] == pad_len) {
            counter++;
        }
    }

    // the bytes vector is unpad if the counter is equal to the pad length 
    // i.e. the byte value of the padding is equal to the pad length
    if (counter == pad_len) {
        // the padding can then be removed by popping the vector pad length times
        for (int i = 0; i < pad_len; i++) {
            bytes.pop_back();
        }
    }
}
