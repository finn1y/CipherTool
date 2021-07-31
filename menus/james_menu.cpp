#include "james_menu.h"
#include "main_menu.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>

james_menu::james_menu(bool& runState, menuPtr& currentMenu): _runState(runState), _currentMenu(currentMenu) {}

james_menu::~james_menu() {}

void james_menu::update() {
    this->print();

    int input = this->handle_int("\nEnter desired menu option: ", 4);

    switch (input) {
        case 1:
            std::cout << "loading Caesars..." << std::endl;
            caesars();
            break;
        
        case 2:
            std::cout << "loading ROT13..." << std::endl;
            ROT13();
            break;

        case 3:
            std::cout << "loading Vigenere..." << std::endl;
            vigenere();
            break;

        case 4:
            // goes back to previous menu
            this->_currentMenu = menuPtr(new main_menu(this->_runState, this->_currentMenu));
            break;
    }
}

void james_menu::print() {
    std::cout << "\n********* James' Section ********" << std::endl;
    std::cout << "|-------------------------------|" << std::endl;
    std::cout << "|\t1: Caesars Cipher \t|" << std::endl;
    std::cout << "|\t2: ROT13 Cipher   \t|" << std::endl;
    std::cout << "|\t3: Vigenere Cipher\t|" << std::endl;
    std::cout << "|\t4: Back           \t|" << std::endl;
    std::cout << "|-------------------------------|" << std::endl;
}

void james_menu::caesars(){
    do{
      
      std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^\n";
      std::cout << " Caesar encryption tool     \n";
      std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^\n";
      std::cout << "------------------------\n";
      std::cout << "Press 1 to Encrypt File\n";
      std::cout << "Press 2 to Decrypt File\n";
      std::cout << "Press 3 to Encrypt Input\n";
      std::cout << "Press 4 to Decrypt Input\n";
      std::cout << "Press 5 to Go Back\n";
      std::cout << "------------------------\n";
      std::cout << "Enter choice: ";
      std::cin >> user_in;
      std::cin.ignore(); //ignores the previous input for future requests
      if (user_in == "1"){

        std::string filename;
        std::cout<<"Please enter the filename you would like to encrypt:\n(Do not inlcude file extension)\n";
        std::cin>>filename;  //save inputted filename
        caesarFile_encrypt(filename);
        
      }//endif
      else if (user_in == "2"){
        std::string filename;
        std::cout<<"Please enter the filename you would like to decrypt:\n(Do not inlcude file extension)\n";
        std::cin>>filename;  //save inputted filename
        caesarFile_decrypt(filename);
      }//endelse
      else if (user_in == "3"){
        std::cout<< "Please enter the text you wish to encrypt:\n";
        getline(std::cin,text);
        caesarInput_encrypt(text);
      }//endelse
      else if (user_in == "4"){
        std::cout<< "Please enter the text you wish to encrypt:\n";
        getline(std::cin,text);
        caesarInput_decrypt(text);
      }//endelse
      else if(user_in == "5"){
        break;
      }
      else{
        std::cout<<"Input is not valid! \nPlease try again:\n\n";
      }
      
    }while((user_in != "1" or user_in !="2" or user_in !="3" or user_in !="4" or user_in !="5"));
}//endmain



///////Encrypt FILES Function///////
void james_menu::caesarFile_encrypt(std::string filename){

  std::ifstream file;  
  std::string line;
  file.open(filename + ".txt"); //open file corresponding to filename
  //create new file to store output
  std::ofstream Encrypted("encrypted_" + filename + ".txt");
  int file_size = file.tellg();


  std::string encrypt_alpha = "abcdefghijklmnopqrstuvwxyz";
  std::string encrypted;    //variable to store new text
  std::string enc_text;     //variable to temp store value
  std::string empty = " ";
  int key;                  //store the key value
  
  std::cout<<"Please enter a key:    ";
  std::cin >> key;
  //While loop ensures input is valid
  while(std::cin.fail()) {
    std::cout << "Input invalid..." << std::endl;
    std::cin.clear();
    std::cin.ignore();
    std::cout<<"Please enter a key:    ";
    std::cin >> key;
  }
  std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
  std::cout<<"Before Encryption: "<<"\n" <<std::endl;

  //iterate user input
  while(getline (file,line)){
    std::cout<<line<<"\n"; 
    for (int i=0; i<line.length(); i++){
      //if whitespace just add a space in the encrypted variable
      if (isspace(line[i])){
          encrypted = encrypted + empty;
      }//endif
      else if (ispunct(line[i]) or isdigit(line[i])){
        encrypted = encrypted + line[i];
      }//endelseif
      else{
        if (isupper(line[i])){
          line[i]=tolower(line[i]);
        }//endif
        //otherwise iterate the encrypt_alpha variable
        for (int j=0;j<26;j++){
          //compare values from text and conv using key
          if (line[i]==encrypt_alpha[j]){
            enc_text = encrypt_alpha[((j+key)%26)];
            encrypted = encrypted + enc_text;
          }//endif
        }//endfor
      }//endelse
    }//endfor
    encrypted = encrypted + "\n"; 
  }//endwhile

  Encrypted << encrypted; //Write to file

  std::cout<<"\nEncrypted with a key of: "<<key<<"\n" <<std::endl;
  std::cout<<"After Encryption: \n"<<encrypted<< std::endl;
  std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<"\n\n" <<std::endl;

}//endfuntion


///////Encrypt Caesars INPUT Function///////
void james_menu::caesarInput_encrypt(std::string text){
  std::string encrypt_alpha = "abcdefghijklmnopqrstuvwxyz";
  std::string encrypted;    //variable to store new text
  int key;                  //store the key value
  //Take in key value from user
  
  std::cout<<"Please enter a key:    ";
  std::cin >> key;
  //While loop ensures input is valid
  while(std::cin.fail()) {
    std::cout << "Input invalid..." << std::endl;
    std::cin.clear();
    std::cin.ignore();
    std::cout<<"Please enter a key:    ";
    std::cin >> key;
  }

  std::string alphabet = encrypt_alpha ;
  encrypted = shift_translate(text, key, alphabet);
  std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
  std::cout<<"Before Encryption: "<<text<<"\n" <<std::endl;
  std::cout<<"Encrypted with a key of: "<<key<<"\n" <<std::endl;
  std::cout<<"After Encryption: "<<encrypted<< std::endl;
  std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<"\n\n" <<std::endl;
}//endfuntion

///////Decrypt Caesars INPUT Function///////
void james_menu::caesarInput_decrypt(std::string text){
  std::string decrypt_alpha = "zyxwvutsrqponmlkjihgfedcba";
  std::string decrypted;    //variable to store new text
  int key;                  //store the key value
  //Take in key value from user
  std::cout<<"Please enter a key:    ";
  std::cin>>key;

  //While loop ensures the input is valid
  while(std::cin.fail()) {
  std::cout << "Input invalid..." << std::endl;
  std::cin.clear();
  std::cin.ignore();
  std::cout<<"Please enter a key:    ";
  std::cin >> key;
  }//endwhile
  std::string alphabet = decrypt_alpha;
  decrypted = shift_translate(text, key, alphabet);
  std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
  std::cout<<"Before Decryption: "<<text<<"\n"<<std::endl;
  std::cout<<"Decrypted with a key of: "<<key<<"\n" <<std::endl;
  std::cout<<"After Decryption: "<<decrypted<<std::endl;
  std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<"\n\n" <<std::endl;
}//endfuntion

///////Decrypt Caesars FILES Function///////
void james_menu::caesarFile_decrypt(std::string filename){

  std::ifstream file;  
  std::string line;
  file.open(filename + ".txt"); //open file corresponding to filename

  //create new file to store output
  std::ofstream Decrypted("decrypted_" + filename + ".txt");
  int file_size = file.tellg();


  std::string decrypt_alpha = "zyxwvutsrqponmlkjihgfedcba";
  std::string decrypted;    //variable to store new text
  std::string dec_text;     //variable to temp store value
  std::string empty = " ";
  int key;                  //store the key value
  //Take in key value from user
  std::cout<<"Please enter a key:    ";
  std::cin>>key;

  //While loop ensures the input is valid
  while(std::cin.fail()) {
    std::cout << "Input invalid..." << std::endl;
    std::cin.clear();
    std::cin.ignore();
    std::cout<<"Please enter a key:    ";
    std::cin >> key;
  }

  std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
  std::cout<<"Before Decryption: "<<"\n"<<std::endl;

  //iterate user input
    while(getline (file,line)){

      std::cout<<line<<"\n"; 
      for (int i=0; i<line.length(); i++){
        //if whitespace just add a space in the encrypted variable
        if (isspace(line[i])){
            decrypted = decrypted + empty;
        }//endif
        else if (ispunct(line[i]) or isdigit(line[i])){
          decrypted = decrypted + line[i];
        }//endelseif
        else{
          if (isupper(line[i])){
            line[i]=tolower(line[i]);
          }//endif
          //otherwise iterate the encrypt_alpha variable
          for (int j=0;j<26;j++){
            //compare values from text and conv using key
            if (line[i]==decrypt_alpha[j]){
              dec_text = decrypt_alpha[((j+key)%26)];
              decrypted = decrypted + dec_text;
            }//endif
          }//endfor
        }//endelse
      }//endfor
    decrypted = decrypted + "\n"; 
  }//endwhile

  Decrypted << decrypted;

  std::cout<<"\nDecrypted with a key of: "<<key<<"\n" <<std::endl;
  std::cout<<"After Decryption: \n"<<decrypted<<std::endl;
  std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<"\n\n" <<std::endl;
}//endfuntion



//SHIFT TRANSLATE//
std::string james_menu::shift_translate(std::string text, int key, std::string alphabet){
  std::string translate_alpha = alphabet;
  std::string translation;    //variable to store new text
  std::string dec_text;     //variable to temp store value
  std::string empty = " ";
  for (int i=0; i<text.length(); i++){
    //if whitespace just add a space in the decrypted variable
    if (isspace(text[i])){
        translation = translation + empty;
    }//endif
    else if (ispunct(text[i]) or isdigit(text[i])){
      translation = translation + text[i];
    }
    else{
      if (isupper(text[i])){
        text[i]=tolower(text[i]);
      }
      //otherwise iterate the encrypt_alpha variable
      for (int j=0;j<26;j++){
        //compare values from text and conv using key
        if (text[i]==translate_alpha[j]){
          dec_text = translate_alpha[((j+key)%26)];
          translation = translation + dec_text;
        }//endif
      }//endfor
    }//endelse
  }//endfor
  return translation;
}//endfunction



void james_menu::ROT13(){
    do{
      
      std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n";
      std::cout << "        ROT13 encryption tool\n";
      std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n";
      std::cout << "-------------------------------------\n";
      std::cout << "Press 1 to encrypt/decrypt input\n";
      std::cout << "Press 2 to encrypt/decrypt file\n";
      std::cout << "Press 3 to return to home\n";
      std::cout << "-------------------------------------\n";
      std::cout << "Enter choice: ";
      std::cin >> user_in;
      std::cin.ignore(); //ignores the previous input for future requests
      if (user_in == "1"){

        std::cout<< "Please enter the text you wish to encrypt:\n";
        getline(std::cin,text);
        ROT13_input(text);
      }
      if(user_in =="2"){
        std::string filename;
        std::cout<<"Please enter the filename you would like to convert:\n(Do not inlcude file extension)\n";
        std::cin>>filename;  //save inputted filename
        ROT13_file(filename);
      }
      ///RETURN HOME///
      else if(user_in == "3"){
        break;
      }
      else{
        std::cout<<"Input is not valid! \nPlease try again:\n\n";
      }
      
    }while((user_in != "1" or user_in !="2"));
}//endmain



///////ROT13 Function///////
void james_menu::ROT13_input(std::string text){

  std::string encrypt_alpha = "abcdefghijklmnopqrstuvwxyz";
    std::string conversion;    //variable to store new text
    std::string rot_text;     //variable to temp store value
    std::string empty = " ";
    int key = 13;
    //iterate user input
    for (int i=0; i<text.length(); i++){
      //if whitespace just add a space in the encrypted variable
      if (isspace(text[i])){
          conversion = conversion + empty;
      }//endif
      else if (ispunct(text[i]) or isdigit(text[i])){
        conversion = conversion + text[i];
      }//endelseif
      else{
        if (isupper(text[i])){
          text[i]=tolower(text[i]);
        }//endif
        //otherwise iterate the encrypt_alpha variable
        for (int j=0;j<26;j++){
          //compare values from text and conv using key
          if (text[i]==encrypt_alpha[j]){
            rot_text = encrypt_alpha[((j+key)%26)];
            conversion = conversion + rot_text;
          }//endif
        }//endfor
      }//endelse
    }//endfor
    std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
    std::cout<<"Before Conversion: "<<text<<"\n" <<std::endl;
    std::cout<<"Encrypted/decrypted with ROT13\n" <<std::endl;
    std::cout<<"After Conversion: "<<conversion<< std::endl;
    std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<"\n\n" <<std::endl;
}//endfuntion

void james_menu::ROT13_file(std::string filename){

  std::ifstream file;  
  std::string line;
  file.open(filename + ".txt"); //open file corresponding to filename

  //create new file to store output
  std::ofstream Converted("converted_" + filename + ".txt");
  int file_size = file.tellg();


  std::string encrypt_alpha = "abcdefghijklmnopqrstuvwxyz";
  std::string converted;    //variable to store new text
  std::string enc_text;     //variable to temp store value
  std::string empty = " ";
  int key = 13;                  //store the key value
  
  std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
  std::cout<<"Before ROT13: " <<std::endl;

  //iterate user input
  while(getline (file,line)){
    std::cout<<line<<"\n"; 

    for (int i=0; i<line.length(); i++){
      //if whitespace just add a space in the encrypted variable
      if (isspace(line[i])){
          converted = converted + empty;
      }//endif
      else if (ispunct(line[i]) or isdigit(line[i])){
        converted = converted + line[i];
      }//endelseif
      else{
        if (isupper(line[i])){
          line[i]=tolower(line[i]);
        }//endif
        //otherwise iterate the encrypt_alpha variable
        for (int j=0;j<26;j++){
          //compare values from text and conv using key
          if (line[i]==encrypt_alpha[j]){
            enc_text = encrypt_alpha[((j+key)%26)];
            converted = converted + enc_text;
          }//endif
        }//endfor
      }//endelse
    }//endfor
    converted = converted + "\n"; 
  }//endwhile

  Converted << converted; //Write to file

  std::cout<<"\nEncrypted/decrypted with ROT13\n" <<std::endl;
  std::cout<<"After ROT13: \n"<<converted<< std::endl;
  std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<"\n\n" <<std::endl;

}//endfuntion

void james_menu::vigenere() {
  std::string AtoZ ="abcdefghijklmnopqrstuvwxyz";
  std::string ZtoA ="zyxwvutsrqponmlkjihgfedcba";
  do{
      
      std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n";
      std::cout << "      Vigenere encryption tool        \n";
      std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n";
      std::cout << "------------------------\n";
      std::cout << "Press 1 to Encrypt Input\n";
      std::cout << "Press 2 to Decrypt Input\n";
      std::cout << "Press 3 to Go Back\n";
      std::cout << "------------------------\n";
      std::cout << "Enter choice: ";
      std::cin >> user_in;
      std::cin.ignore(); //ignores the previous input for future requests
      if (user_in == "1"){
        std::cout<< "Please enter the text you wish to encrypt:\n";
        getline(std::cin,text);
        vigenere_Input(text, AtoZ);
      }//endif
      else if (user_in == "2"){
        std::cout<< "Please enter the text you wish to decrypt:\n";
        getline(std::cin,text);
        vigenere_Input(text, ZtoA);
      }//endelse
      else if(user_in == "3"){
        break;
      }
      else{
        std::cout<<"Input is not valid! \nPlease try again:\n\n";
      }
      
    }while((user_in != "1" or user_in !="2" or user_in !="3"));

}//endfunction


void james_menu::vigenere_Input(std::string textIn, std::string alphabet){
  std::string keyIn;
  std::cout<<"\nPlease enter a key phrase: \n";
  std::cout<<"(Do not inlcude spaces)\n";
  std::cin>>keyIn;
  std::string keyLength = makekey(keyIn,textIn);
  wordtonum(keyLength);
  std::string encrypted = vigenere_translate(textIn, alphabet);
  std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
  std::cout<<"Before: "<<textIn<<"\n" <<std::endl;
  std::cout<<"Performed with a key of: "<<keyIn<<"\n" <<std::endl;
  std::cout<<"After: "<<encrypted<< std::endl;
  std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<"\n\n" <<std::endl;
}


std::string james_menu::makekey(std::string key, std::string text){
  std::string newkey = "";
  int textlen = text.length();
  int repeat = round((text.length()/key.length())+1);
  for (int i=1; i<=repeat; i++){
    newkey = newkey + key;
  }
  while(newkey.length() > textlen){
    newkey.pop_back();
  }
  return newkey;
}

//Vigenere TRANSLATE//
std::string james_menu::vigenere_translate(std::string text, std::string alphabet){
  std::string translation;    //variable to store new text
  std::string vigenere;     //variable to temp store value
  std::string empty = " ";

  int counter = 0;

  for (int i=0; i<text.length(); i++){
    //if whitespace just add a space in the decrypted variable
    if (isspace(text[i])){
        translation = translation + empty;
    }//endif
    else if (ispunct(text[i]) or isdigit(text[i])){
      translation = translation + text[i];
    }
    else{
      if (isupper(text[i])){
        text[i]=tolower(text[i]);
      }
      
      //otherwise iterate the encrypt_alpha variable
      for (int x=0;x<26;x++){
        //compare values from text and conv using key
        if (text[i]==alphabet[x]){
          vigenere = alphabet[((x+dynkey[(counter)]-1)%26)];
          translation = translation + vigenere;
        }//endif
      }//endfor
      counter = counter +1;
    }//endelse
  }//endfor
  return translation;
}//endfunction



void james_menu::wordtonum(std::string keyword){
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

  dynkey = key;
}//endfunction
