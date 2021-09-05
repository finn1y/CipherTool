#!/usr/bin/env bash

if [ ! -f bin/cipher_tool ]; then
    echo "bin/cipher_tool not found. Run make in src directory to build before running tests"
    exit 1
fi 

if [ ! -d test_suite/txt ]; then
    mkdir -p test_suite/txt
fi

if [ ! -f test_suite/txt/plaintext.txt ]; then
    echo "Egg blah blah" > test_suite/txt/plaintext.txt
fi

if [ -f test_suite/txt/plaintext.txt ]; then
    tr [a-z] [A-Z] < test_suite/txt/plaintext.txt > test_suite/txt/upper_plaintext.txt
fi

CIPHERS=( "XOR" "AES" "OTP" "Caesars" "ROT13" "Vigenere" )

for cipher in ${CIPHERS[@]}; do
    echo "Testing $cipher from command line"

    ENCRYPT="./bin/cipher_tool --cipher $cipher --encrypt --input-file test_suite/txt/plaintext.txt --output-file test_suite/txt/ciphertext.txt"

    echo "$ENCRYPT"
    KEY=$(eval "$ENCRYPT" | cut -d' ' -f2)

    DECRYPT="./bin/cipher_tool --cipher $cipher --decrypt --key $KEY --input-file test_suite/txt/ciphertext.txt --output-file test_suite/txt/decrypted_plaintext.txt"
    echo "$DECRYPT"
    eval "$DECRYPT"

    if [ "$cipher" == "OTP" ]; then 
        INPUT_TXT=$(cat test_suite/txt/upper_plaintext.txt)
    else
        INPUT_TXT=$(cat test_suite/txt/plaintext.txt)
    fi
    OUTPUT_TXT=$(cat test_suite/txt/decrypted_plaintext.txt)

    if [ "${OUTPUT_TXT}" == "${INPUT_TXT}" ]; then
        echo "Test passed"
    else 
        echo "Test failed"
    fi
done

exit 0
