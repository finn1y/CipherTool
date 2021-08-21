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
OBJS=xor.o aes.o otp.o

all: ${OBJS}

otp.o: otp.cpp otp.h
	${CC} ${CFLAGS} -c $^

aes.o: aes.cpp aes.h
	${CC} ${CFLAGS} -c $^

xor.o: xor.cpp xor.h
	${CC} ${CFLAGS} -c $^

.PHONY: clean

clean:
	${CLEAN_CMD}
