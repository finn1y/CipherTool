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
    tr [A-Z] [a-z] < test_suite/txt/plaintext.txt > test_suite/txt/lower_plaintext.txt
fi

CIPHERS=( "XOR" "AES" "OTP" "Caesars" "ROT13" "Vigenere" )
KEYS=( "password" "password" "1" "5" "5" "password")

SIZE=$(stat -c%s test_suite/txt/plaintext.txt)
i=0

while [ $i -lt $SIZE ]; do
    KEYS[2]+="$i"
    i=$((i+1))
done

for i in {0..5}; do
    echo "Testing ${CIPHERS[${i}]} from command line"

    ENCRYPT="./bin/cipher_tool --cipher ${CIPHERS[${i}]} --encrypt --key ${KEYS[${i}]} --input-file test_suite/txt/plaintext.txt --output-file test_suite/txt/ciphertext.txt"
    DECRYPT="./bin/cipher_tool --cipher ${CIPHERS[${i}]} --decrypt --key ${KEYS[${i}]} --input-file test_suite/txt/ciphertext.txt --output-file test_suite/txt/decrypted_plaintext.txt"
    
    echo "$ENCRYPT"; eval "$ENCRYPT"
    echo "$DECRYPT"; eval "$DECRYPT"

    if [ "${CIPHERS[${i}]}" == "OTP" ]; then 
        INPUT_TXT="test_suite/txt/upper_plaintext.txt"
    elif [ "${CIPHERS[${i}]}" == "Caesars" -o "${CIPHERS[${i}]}" == "ROT13" -o "${CIPHERS[${i}]}" == "Vigenere" ]; then
        INPUT_TXT="test_suite/txt/lower_plaintext.txt"
    else
        INPUT_TXT="test_suite/txt/plaintext.txt"
    fi

    OUTPUT_TXT="test_suite/txt/decrypted_plaintext.txt"
    DIFF=$(diff -aZ $INPUT_TXT $OUTPUT_TXT)

    if [ "$DIFF" == "" ]; then
        echo "Test passed"
    else 
        echo "Test failed"

        echo "$DIFF"
    fi
done

exit 0
