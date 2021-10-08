#!/usr/bin/env bash

#Check if binary exists as required for tests
if [ ! -f bin/cipher_tool ]; then
    echo "bin/cipher_tool not found. Run make in src directory to build before running tests"
    exit 1
fi 

#Create and populate test data
if [ ! -d test_suite/txt ]; then
    mkdir -p test_suite/txt
fi

if [ ! -f test_suite/txt/plaintext.txt ]; then
    echo "Egg blah blah" > test_suite/txt/plaintext.txt
fi

#Some ciphers decrypt to all caps or no caps
#Produce test data in all caps or no caps for pass/fail check
if [ -f test_suite/txt/plaintext.txt ]; then
    tr [a-z] [A-Z] < test_suite/txt/plaintext.txt > test_suite/txt/upper_plaintext.txt
    tr [A-Z] [a-z] < test_suite/txt/plaintext.txt > test_suite/txt/lower_plaintext.txt
fi

#Arrays of available ciphers and associated keys 
#Index of cipher matches index of the associated key
CIPHERS=( "XOR" "AES" "OTP" "Caesars" "ROT13" "Vigenere" )
KEYS=( "password" "password" "1" "5" "5" "password")

#Variables for tests
SIZE=$(stat -c%s test_suite/txt/plaintext.txt)
i=0

#OTP requires key at least size in bytes of input otherwise data is lost
#Extend OTP key to be size of the input in bytes
while [ $i -lt $SIZE ]; do
    KEYS[2]+="$i"
    i=$((i+1))
done

#Test each cipher with associated key
for i in {0..5}; do
    echo "Testing ${CIPHERS[${i}]} from command line"

    #Encrypt test data then decrypt the output 
    ENCRYPT="./bin/cipher_tool --cipher ${CIPHERS[${i}]} --encrypt --key ${KEYS[${i}]} --input-file test_suite/txt/plaintext.txt --output-file test_suite/txt/ciphertext.txt"
    DECRYPT="./bin/cipher_tool --cipher ${CIPHERS[${i}]} --decrypt --key ${KEYS[${i}]} --input-file test_suite/txt/ciphertext.txt --output-file test_suite/txt/decrypted_plaintext.txt"
    
    echo "$ENCRYPT"; eval "$ENCRYPT"
    echo "$DECRYPT"; eval "$DECRYPT"

    #Check for pass
    if [ "${CIPHERS[${i}]}" == "OTP" ]; then 
        #For OTP decrypted is in all CAPS
        INPUT_TXT="test_suite/txt/upper_plaintext.txt"
    elif [ "${CIPHERS[${i}]}" == "Caesars" -o "${CIPHERS[${i}]}" == "ROT13" -o "${CIPHERS[${i}]}" == "Vigenere" ]; then
        #For Caesars, ROT13 and Vigenere decrypted has no CAPS
        INPUT_TXT="test_suite/txt/lower_plaintext.txt"
    else
        #All other ciphers decrypted should match test data exactly
        INPUT_TXT="test_suite/txt/plaintext.txt"
    fi

    #Diff the decrypted data and test data
    OUTPUT_TXT="test_suite/txt/decrypted_plaintext.txt"
    DIFF=$(diff -aZ $INPUT_TXT $OUTPUT_TXT)

    #If any differences between decrypted data and test data then cipher has failed
    #Either encryption failed or decryption failed
    if [ "$DIFF" == "" ]; then
        echo "Test passed"
    else 
        echo "Test failed"
        #If test failed then echo the differences for debugging info
        echo "$DIFF"
    fi
done

exit 0
