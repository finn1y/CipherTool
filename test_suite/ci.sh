#!/usr/bin/env bash

#Move to repo root
cd `dirname $0`/../

##Clean workspace
echo "Cleaning Workspace"
make -C src clean_all

if [ -f bin/cipher_tool ]; then
    rm -rf bin/cipher_tool
fi

if [ -e test_suite/txt/* ]; then
    rm -rf test_suite/txt/*
fi

if [ -f test_status ]; then
    rm -rf test_status
fi

#Build CipherTool
echo "Building CipherTool"
make -C src all

#Build Smoke tests
if [ ! -d test_suite/txt ]; then
    mkdir -p test_suite/txt
fi

#Populate test data
echo "Black pudding is a distinct regional type of blood sausage originating in Great Britain and Ireland. It is made from pork or beef blood, with pork fat or beef suet, and a cereal, usually oatmeal, oat groats or barley groats. The high proportion of cereal, along with the use of certain herbs such as pennyroyal, serves to distinguish black pudding from blood sausages eaten in other parts of the world.[1]" > test_suite/txt/plaintext.txt

#Smoke tests
echo "Running smoke tests"
./test_suite/gui_test.exp XOR test_suite/txt/plaintext.txt | tee -a test_status
./test_suite/gui_test.exp AES test_suite/txt/plaintext.txt | tee -a test_status
./test_suite/gui_test.exp OTP test_suite/txt/plaintext.txt | tee -a test_status
./test_suite/cmd_test.sh | tee -a test_status

#Check for pass or fail
FAILS=`grep -c "Test failed" test_status`

#If any fails then CI should fail
if [ $FAILS -gt 0 ]; then
    echo "Failed $FAILS tests"
    #Exit non-zero for fail
    exit 1
fi

#Remove files created by tests
rm -rf test_suite/txt/*

#Exit zero for pass
exit 0
