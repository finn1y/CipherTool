ifeq (${OS}, Windows_NT)
	CLEAN_CMD := DEL /F /Q *.o *.gch
else
	ifeq ($(shell uname -s), Linux)
		CLEAN_CMD := rm -f *.o *.gch
	else ifeq ($(shell uname -s), Darwin)
		CLEAN_CMD := rm -f *.o *.gch
	endif
endif

CC=g++
CFLAGS=-Wall -g
OBJS=application.o main.o 
COMPILED_OBJS=ciphers/aes.o ciphers/otp.o ciphers/xor.o ciphers/coder.o \
			menus/aes_menu.o menus/finn_menu.o menus/james_menu.o \
			menus/jj_menu.o menus/main_menu.o menus/menu.o menus/xor_menu.o
TARGET=../bin/cipher_tool

all: ${TARGET}

${TARGET}: ciphers menus ${OBJS}
	${CC} ${CFLAGS} ${COMPILED_OBJS} ${OBJS} -o $@ 

main.o: main.cpp 
	${CC} ${CFLAGS} -c $^

application.o: application.cpp application.h
	${CC} ${CFLAGS} -c $^

.PHONY: menus ciphers \
		clean_all clean clean_menus clean_ciphers

menus:
	make -C menus all

ciphers:
	make -C ciphers all

clean_all: clean clean_menus clean_ciphers

clean:
	${CLEAN_CMD}

clean_menus:
	make -C menus clean

clean_ciphers:
	make -C ciphers clean

