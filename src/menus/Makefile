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
OBJS=menu.o main_menu.o finn_menu.o jj_menu.o james_menu.o xor_menu.o aes_menu.o

all: ${OBJS} 

aes_menu.o: aes_menu.cpp menu.cpp menu.h aes_menu.h
	${CC} ${CFLAGS} -c $^

xor_menu.o: xor_menu.cpp menu.cpp menu.h xor_menu.h 
	${CC} ${CFLAGS} -c $^
	
james_menu.o: james_menu.cpp menu.cpp menu.h james_menu.h
	${CC} ${CFLAGS} -c $^

jj_menu.o: jj_menu.cpp menu.cpp menu.h jj_menu.h
	${CC} ${CFLAGS} -c $^

finn_menu.o: finn_menu.cpp menu.cpp menu.h finn_menu.h 
	${CC} ${CFLAGS} -c $^

main_menu.o: main_menu.cpp menu.cpp menu.h main_menu.h 
	${CC} ${CFLAGS} -c $^

menu.o: menu.cpp menu.h
	${CC} ${CFLAGS} -c $^

.PHONY: clean

clean:
	${CLEAN_CMD}	
