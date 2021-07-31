CC = g++
CFLAGS = -Wall -g
OBJS = xor.o aes.o otp.o menu.o main_menu.o finn_menu.o jj_menu.o james_menu.o xor_menu.o aes_menu.o application.o main.o 

cipher_tool: ${OBJS}
	${CC} ${CFLAGS} $^ -o $@ 

main.o: main.cpp 
	${CC} ${CFLAGS} -c $^

application.o: application.cpp application.h
	${CC} ${CFLAGS} -c $^

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

otp.o: otp.cpp otp.h
	${CC} ${CFLAGS} -c $^

aes.o: aes.cpp aes.h
	${CC} ${CFLAGS} -c $^

xor.o: xor.cpp xor.h
	${CC} ${CFLAGS} -c $^

.PHONY: clean

clean:
	rm *.o
	rm *.gch
