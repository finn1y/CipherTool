#!/usr/bin/env bash

if [ ! -f bin/cipher_tool ]; then
    echo "bin/cipher_tool not found. Run make in src directory to build before running tests"
    exit 1
fi 

CIPHERS=( "XOR" "AES" "OTP" "Caesars" "ROT13" "Vigenere" )

if [ ! -d test_suite/txt ]; then
    mkdir -p test_suite/txt
fi

if [ ! -f test_suite/txt/plaintext.txt ]; then
    echo "Egg blah blah" > test_suite/txt/plaintext.txt
fi

for cipher in ${CIPHERS[@]}; do
    echo "Testing $cipher from command line"

    ./bin/cipher_tool --cipher $cipher --encrypt --key 1234 --input-file test_suite/txt/plaintext.txt --output-file test_suite/txt/ciphertext.txt
    ./bin/cipher_tool --cipher $cipher --decrypt --key 1234 --input-file test_suite/txt/ciphertext.txt --output-file test_suite/txt/decrypted_plaintext.txt

    INPUT_TXT=$(cat test_suite/txt/plaintext.txt)
    OUTPUT_TXT=$(cat test_suite/txt/decrypted_plaintext.txt)

    if [ "${OUTPUT_TXT}" == "${INPUT_TXT}" ]; then
        echo "Test passed"
    else 
        echo "Test failed"
    fi
done

exit 0
