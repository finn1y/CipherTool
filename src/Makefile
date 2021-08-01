CC=g++
CFLAGS=-Wall -g
OBJS=xor.o aes.o otp.o menu.o main_menu.o finn_menu.o jj_menu.o james_menu.o xor_menu.o aes_menu.o application.o main.o 
TARGET=../bin/cipher_tool

all: ${TARGET}

${TARGET}: ${OBJS}
	${CC} ${CFLAGS} $^ -o $@ 

main.o: main.cpp 
	${CC} ${CFLAGS} -c $^

application.o: application.cpp application.h
	${CC} ${CFLAGS} -c $^

aes_menu.o: menus/aes_menu.cpp menus/menu.cpp menus/menu.h menus/aes_menu.h
	${CC} ${CFLAGS} -c $^

xor_menu.o: menus/xor_menu.cpp menus/menu.cpp menus/menu.h menus/xor_menu.h 
	${CC} ${CFLAGS} -c $^
	
james_menu.o: menus/james_menu.cpp menus/menu.cpp menus/menu.h menus/james_menu.h
	${CC} ${CFLAGS} -c $^

jj_menu.o: menus/jj_menu.cpp menus/menu.cpp menus/menu.h menus/jj_menu.h
	${CC} ${CFLAGS} -c $^

finn_menu.o: menus/finn_menu.cpp menus/menu.cpp menus/menu.h menus/finn_menu.h 
	${CC} ${CFLAGS} -c $^

main_menu.o: menus/main_menu.cpp menus/menu.cpp menus/menu.h menus/main_menu.h 
	${CC} ${CFLAGS} -c $^

menu.o: menus/menu.cpp menus/menu.h
	${CC} ${CFLAGS} -c $^

otp.o: ciphers/otp.cpp ciphers/otp.h
	${CC} ${CFLAGS} -c $^

aes.o: ciphers/aes.cpp ciphers/aes.h
	${CC} ${CFLAGS} -c $^

xor.o: ciphers/xor.cpp ciphers/xor.h
	${CC} ${CFLAGS} -c $^

.PHONY: clean

clean:
	rm */*.o *.o */*.gch *.gch ${TARGET}
