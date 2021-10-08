#include "coder.h"

Coder::Coder() {
    //cipher defaults to XOR
    this->_cipher = Xor;
    //key defaults to empty and therefore random
    this->_key = "";    
}

Coder::Coder(Ciphers cipher, std::string key): _cipher(cipher), _key(key) {}

Coder::~Coder() {}

void Coder::set_cipher(Ciphers cipher) {
    this->_cipher = cipher;
}

void Coder::set_key(std::string key) {
    this->_key = key;
}

std::string Coder::encrypt(std::string plaintext) {
    std::string ciphertext;

    switch (this->_cipher) {
        case Xor:
            //0 argument for encryption
            ciphertext = this->xor_cipher(0, plaintext);
            break;

        case Aes:
            ciphertext = this->aes_cipher(0, plaintext);
            break;

        case Otp:
            ciphertext = this->otp_cipher(0, plaintext);
            break;

        case Caesars:
            ciphertext = this->caesars_cipher(0, plaintext);
            break;

        case Rot13:
            ciphertext = this->rot13_cipher(0, plaintext);
            break;

        case Vigenere:
            ciphertext = this->vigenere_cipher(0, plaintext);
            break;
    }
    
    return ciphertext;
}

std::string Coder::decrypt(std::string ciphertext) {
    std::string plaintext;

    switch (this->_cipher) {
        case Xor:
            //1 argument to perform decryption
            plaintext = this->xor_cipher(1, ciphertext);
            break;

        case Aes:
            plaintext = this->aes_cipher(1, ciphertext);
            break;

        case Otp:
            plaintext = this->otp_cipher(1, ciphertext);
            break;

        case Caesars:
            plaintext = this->caesars_cipher(1, ciphertext);
            break;

        case Rot13:
            plaintext = this->rot13_cipher(1, ciphertext);
            break;

        case Vigenere:
            plaintext = this->vigenere_cipher(1, ciphertext);
            break;
    }
    
    return plaintext;
}

std::string Coder::xor_cipher(int en_de, std::string input) {
    std::vector<uint8_t> bytes; 
    
    if (!en_de) {
        //encrypt
        if (!input.empty()) {
            // the user inputted string is transformed into the vector of bytes to be used in the xor object
            bytes.resize(input.size());
            for (size_t i = 0; i < input.size(); i++) {
                bytes[i] = input.at(i);
            }
        }
    }
    if (en_de) {
        //decrypt
        // the ciphertext is converted into the vector of bytes
        for (size_t i = 0; i < input.size(); i += 2) {
            std::string sub_str = input.substr(i, 2);
            uint8_t byte = std::stoi(sub_str, 0, 16);
            bytes.push_back(byte); 
        }
    }

    this->_xor = new XOR(bytes);
    if (this->_key.empty()) {
        this->_xor->gen_key();
    }
    else {
        this->_xor->set_key(this->_key);
    }

    this->_xor->encrypt();

    std::stringstream output;
    // the encrypted ciphertext or decrypted plaintext is outputted to the console for the user to see
    if (!en_de) {
        // ciphertext is printed in hexadecimal for readability
        for (size_t i = 0; i < bytes.size(); i++) {
            output << std::hex << std::setw(2) << std::setfill('0') << (int)bytes[i] << std::dec;
        }
    }
    if (en_de) {
        for (size_t i = 0; i < bytes.size(); i++) {
            output << bytes[i];
        }
    }
    return output.str();
}

std::string Coder::aes_cipher(int en_de, std::string input) {
    std::vector<uint8_t> bytes;

    if (!en_de) {
        //encrypt
        if (!input.empty()) {
            // the user inputted string is transformed into the vector of bytes to be used in the xor object
            bytes.resize(input.size());
            for (size_t i = 0; i < input.size(); i++) {
                bytes[i] = input.at(i);
            }
        }
    }
    else if (en_de) {
        //decrypt
        // the ciphertext is converted into the vector of bytes
        for (size_t i = 0; i < input.size(); i += 2) {
            std::string sub_str = input.substr(i, 2);
            uint8_t byte = std::stoi(sub_str, 0, 16);
            bytes.push_back(byte); 
        }
    }

    this->_aes = new aes(bytes);
    if (this->_key.empty()) {
        this->_aes->gen_key();
    }
    else {
        this->_aes->set_key(this->_key);
    }

    std::stringstream output;
    // the input is then encrypted or decrypted and printed to the command line
    if (!en_de) {
        this->_aes->encrypt(); // aes encryption is carried out
        // ciphertext is printed in hexadecimal for readability
        for (size_t i = 0; i < bytes.size(); i++) {
            output << std::hex << std::setw(2) << std::setfill('0') << (int)bytes[i] << std::dec;
        }
    }
    if (en_de) {
        this->_aes->decrypt(); // aes decryption is carried out
        for (size_t i = 0; i < bytes.size(); i++) {
            output << bytes[i];
        }
    }

    return output.str();
}

std::string Coder::otp_cipher(int en_de, std::string input) {
	//Clear and set variables for the output - The encrypted text
	std::string output = "";
    std::string key;
	int keyCounter = 0;
    this->_otp = new OTP;

    if (this->_key.empty()) {
	    srand(time(NULL));
    
    	for (size_t i = 0; i < input.size(); i++) {
		    int randInt = rand() % 36;
	    	key.push_back(this->_otp->alphaNum[randInt]);
    	}

        std::cout << "Key: " << key << std::endl;
    }
    else {
        key = this->_key;
    }
	//Iterate through each character in the plain text
	for (size_t i = 0; i < input.length(); i++) {
		
		//If the character is not alphanumeric (e.g symbol or special character), add it directly to output
		if (_otp->acceptedChar.find(input[i]) == -1) {
			output.push_back(input[i]);

		}
		
		//If the character is alphanumeric use the onetime pad in _otp.h to encrypt
		else {
			std::string Temp = "";
			Temp.push_back(input[i]);

			std::string keyTemp = "";
			keyTemp.push_back(key[keyCounter]);

			//Move to next character in key (prevents program from trying to encrypt unnecessary characters, such as
			//symbols and special characters
			keyCounter++;

			//Add each individual encrypted character to the output string
            if (!en_de) {
			    output.push_back(_otp->encodeChar(Temp, keyTemp));
            }
            else if (en_de) {
			    output.push_back(_otp->decodeChar(Temp, keyTemp));
            }
		}
	}
	
    return output;
}


std::string Coder::caesars_cipher(int en_de, std::string input) {
    std::string translate_alpha;
    std::string translation;    //variable to store new text
    std::string dec_text;     //variable to temp store value
    std::string empty = " ";
    int key;

    if (this->_key.empty()) {
        key = rand();
        std::cout << "Key: " << key << std::endl;
    }
    else { 
        key = std::stoi(this->_key);
    }
    
    if (!en_de) {
        translate_alpha = "abcdefghijklmnopqrstuvwxyz";
    }
    else if (en_de) {
        translate_alpha = "zyxwvutsrqponmlkjihgfedcba";
    }

    for (int i=0; i< input.length(); i++){
        //if whitespace just add a space in the decrypted variable
        if (isspace(input[i])){
            translation = translation + empty;
        }//endif
        else if (ispunct(input[i]) or isdigit(input[i])){
            translation = translation + input[i];
        }
        else {
            if (isupper(input[i])){
                input[i]=tolower(input[i]);
            }
            //otherwise iterate the encrypt_alpha variable
            for (int j=0;j<26;j++){
                //compare values from text and conv using key
                if (input[i]==translate_alpha[j]){
                    dec_text = translate_alpha[((j+key)%26)];
                    translation = translation + dec_text;
                }//endif
            }//endfor
        }//endelse
    }//endfor

    return translation;
}

std::string Coder::rot13_cipher(int en_de, std::string input) {
    std::string encrypt_alpha = "abcdefghijklmnopqrstuvwxyz";
    std::string conversion;    //variable to store new text
    std::string rot_text;     //variable to temp store value
    std::string empty = " ";
    int key = 13;
    //iterate user input
    for (int i=0; i<input.length(); i++){
          //if whitespace just add a space in the encrypted variable
        if (isspace(input[i])){
            conversion = conversion + empty;
        }//endif
        else if (ispunct(input[i]) or isdigit(input[i])){
            conversion = conversion + input[i];
        }//endelseif
        else{
            if (isupper(input[i])){
                input[i]=tolower(input[i]);
            }//endif
            //otherwise iterate the encrypt_alpha variable
            for (int j=0;j<26;j++){
                //compare values from text and conv using key
                if (input[i]==encrypt_alpha[j]){
                    rot_text = encrypt_alpha[((j+key)%26)];
                    conversion = conversion + rot_text;
                }//endif
            }//endfor
        }//endelse
    }//endfor

    return conversion;
}

std::string Coder::vigenere_cipher(int en_de, std::string input) {
    std::string alphabet;

    if (!en_de) {
        alphabet = "abcdefghijklmnopqrstuvwxyz";
    }
    else if (en_de) {
        alphabet = "zyxwvutsrqponmlkjihgfedcba";
    }

    std::string translation;    //variable to store new text
    std::string vigenere;     //variable to temp store value
    std::string empty = " ";
    int counter = 0;

    std::vector<int> dynkey = this->vigenere_key(this->makekey(input));
    for (int i=0; i<input.length(); i++){
        //if whitespace just add a space in the decrypted variable
        if (isspace(input[i])){
            translation = translation + empty;
        }//endif
        else if (ispunct(input[i]) or isdigit(input[i])){
            translation = translation + input[i];
        }
        else{
            if (isupper(input[i])){
                input[i]=tolower(input[i]);
            }
      
            //otherwise iterate the encrypt_alpha variable
            for (int x=0;x<26;x++){
                //compare values from text and conv using key
                if (input[i]==alphabet[x]){
                    vigenere = alphabet[((x+dynkey[(counter)]-1)%26)];
                    translation = translation + vigenere;
                }//endif
            }//endfor
            counter = counter +1;
        }//endelse
    }//endfor
    
    return translation;
}

std::string Coder::makekey(std::string input) {
    std::string newkey = "";
    int textlen = input.length();
    int repeat = round((input.length()/this->_key.length())+1);
    for (int i=1; i<=repeat; i++){
        newkey = newkey + this->_key;
    }
    while(newkey.length() > textlen){
        newkey.pop_back();
    }
    return newkey;
}

std::vector<int> Coder::vigenere_key(std::string keyword) {
    std::string alphabet ="abcdefghijklmnopqrstuvwxyz";
    int length = keyword.length();
    std::vector<int> key;
    for (int i=0; i< keyword.length(); i++){
        for (int j=0;j<26;j++){
            if (keyword[i]==alphabet[j]){
                key.push_back(j+1);
            }//endif
            else if (isspace(keyword[i])){
                key.push_back(0);
            }
        }//endfor
    }//endfor

    return key;
}
