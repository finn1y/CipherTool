#ifndef OTP_H
#define OTP_H

#include <map>
#include <string>
#include <cctype>

class OTP {
public:
	OTP();
	char decodeChar(std::string text, std::string key);
	char encodeChar(std::string text, std::string key);

	std::string encodedString;
	std::string decodedString;
	std::string key;
	std::string generatedKey;

	//Only characters which will be encrypted or decrypted
	//Only characters which are accepted in the key
	std::string acceptedChar = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	std::string alphaNum = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";



private:
	std::map<std::string, std::string> decryptor;
	
	


};

#endif // !OTP_H
