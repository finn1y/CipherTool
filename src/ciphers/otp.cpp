#include "otp.h"

OTP::OTP() {

	//Alphanumeric table to iterate over when decrypting or encrypting
	decryptor["A"] = "abcdefghijklmnopqrstuvwxyz0123456789";
	decryptor["B"] = "bcdefghijklmnopqrstuvwxyz0123456789a";
	decryptor["C"] = "cdefghijklmnopqrstuvwxyz0123456789ab";
	decryptor["D"] = "defghijklmnopqrstuvwxyz0123456789abc";
	decryptor["E"] = "efghijklmnopqrstuvwxyz0123456789abcd";
	decryptor["F"] = "fghijklmnopqrstuvwxyz0123456789abcde";
	decryptor["G"] = "ghijklmnopqrstuvwxyz0123456789abcdef";
	decryptor["H"] = "hijklmnopqrstuvwxyz0123456789abcdefg";
	decryptor["I"] = "ijklmnopqrstuvwxyz0123456789abcdefgh";
	decryptor["J"] = "jklmnopqrstuvwxyz0123456789abcdefghi";
	decryptor["K"] = "klmnopqrstuvwxyz0123456789abcdefghij";
	decryptor["L"] = "lmnopqrstuvwxyz0123456789abcdefghijk";
	decryptor["M"] = "mnopqrstuvwxyz0123456789abcdefghijkl";
	decryptor["N"] = "nopqrstuvwxyz0123456789abcdefghijklm";
	decryptor["O"] = "opqrstuvwxyz0123456789abcdefghijklmn";
	decryptor["P"] = "pqrstuvwxyz0123456789abcdefghijklmno";
	decryptor["Q"] = "qrstuvwxyz0123456789abcdefghijklmnop";
	decryptor["R"] = "rstuvwxyz0123456789abcdefghijklmnopq";
	decryptor["S"] = "stuvwxyz0123456789abcdefghijklmnopqr";
	decryptor["T"] = "tuvwxyz0123456789abcdefghijklmnopqrs";
	decryptor["U"] = "uvwxyz0123456789abcdefghijklmnopqrst";
	decryptor["V"] = "vwxyz0123456789abcdefghijklmnopqrstu";
	decryptor["W"] = "wxyz0123456789abcdefghijklmnopqrstuv";
	decryptor["X"] = "xyz0123456789abcdefghijklmnopqrstuvw";
	decryptor["Y"] = "yz0123456789abcdefghijklmnopqrstuvwx";
	decryptor["Z"] = "z0123456789abcdefghijklmnopqrstuvwxy";
	decryptor["0"] = "0123456789abcdefghijklmnopqrstuvwxyz";
	decryptor["1"] = "123456789abcdefghijklmnopqrstuvwxyz0";
	decryptor["2"] = "23456789abcdefghijklmnopqrstuvwxyz01";
	decryptor["3"] = "3456789abcdefghijklmnopqrstuvwxyz012";
	decryptor["4"] = "456789abcdefghijklmnopqrstuvwxyz0123";
	decryptor["5"] = "56789abcdefghijklmnopqrstuvwxyz01234";
	decryptor["6"] = "6789abcdefghijklmnopqrstuvwxyz012345";
	decryptor["7"] = "789abcdefghijklmnopqrstuvwxyz0123456";
	decryptor["8"] = "89abcdefghijklmnopqrstuvwxyz01234567";
	decryptor["9"] = "9abcdefghijklmnopqrstuvwxyz012345678";



};

//Decode individual character
char OTP::decodeChar(std::string text, std::string key) {

	//Make sure input is the same case as characters in decryption table
	text = tolower(text[0]);
	key = toupper(key[0]);

	//Go to row in table that matches the character in the key
	//Find the character from encrypted text in that row and store it's position
	int intermediate = decryptor[key].find(text);

	//Return character at the stored position in standard order alphanumeric text
	return alphaNum[intermediate];

};

char OTP::encodeChar(std::string text, std::string key) {

	//Make sure input is the same case as characters in decryption table
	text = toupper(text[0]);
	key = toupper(key[0]);

	char encryptedChar = decryptor[key][alphaNum.find(text)];
	return toupper(encryptedChar);

}
