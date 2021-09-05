# CipherTool

Command line interface gui tool for encrypting and decrypting files or standard input using several different ciphers

Project by: Finn, JJ & James

## Help

```
Usage: cipher_tool [OPTION]
Options:
  -c, --cipher=CIPHER           cipher to encrypt/decrypt with defaults to XOR
  -d, --decrypt                 tool will decrypt the input
  -e, --encrypt                 tool will encrypy the input
  -g, --gui                     use the tool with gui
  -h, --help                    shows this message
  -i, --input-file=IN_FILE      file to read input data from
  -k, --key=KEY                 key used by some ciphers will defualt to random key
                                see Ciphers for specific info on keys required for each cipher
  -o, --output-file=OUT_FILE    file to write output data to

Ciphers:
  XOR           KEY of any length as a string
  AES           KEY of any length as a string
  OTP           KEY
  Caesars       KEY as an integer
  ROT13         no KEY taken
  Vigenere      KEY of any length as a string
```

## Install
1. Clone the repo:

```
git clone https://github.com/finn1y/CipherTool.git
```
2. Navigate to the bin directory
3. Run the binary for your system:

      Linux
      ```
      ./cipher_tool-linux64 --gui
      ```
      Windows
      ```
      .\cipher_tool-win64.exe --gui
      ```
4. Happy encrypting!

## Build your own binary

To build your own binary you must have [make](http://gnuwin32.sourceforge.net/install.html) on your machine.

1. Clone the repo:

```
git clone https://github.com/finn1y/CipherTool.git
```
2. Navigate to the src directory
3. Run: 

```
make all
```
4. Navigate to the bin directory
5. Run: 

```
./cipher_tool --gui
```
6. Happy encrypting!
