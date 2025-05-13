CFLAGS = -I. -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -std=c++11 -pthread
CC = g++

final: editor.o
	@echo "linking to final"
	@${CC} ${CFLAGS} editor.o -o struktogrammeditor.bin

editor.o: editor.cpp
	@echo "compiling editor"
	@${CC} ${CFLAGS} -c editor.cpp -o editor.o

clean:
	rm editor.o
