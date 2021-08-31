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

    OUTPUT=$(./bin/cipher_tool --cipher $cipher --key 1234 --file test_suite/txt/plaintext.txt | grep "cipher")

    if [ ${OUTPUT:7} == $cipher ]; then
        echo "Test passed"
    else 
        echo "Test failed"
    fi
done

exit 0
