CC = g++ 
CFLAGS = -I. -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -std=c++11 -pthread

CLI: dHCLI.cpp 
	@echo "Making CLI version"
	@${CC} dHCLI.cpp -o downloadHelperCLI
	@chmod +x downloadHelperCLI

GUI: dH.cpp
	@echo "Making GUI version"
	@${CC} ${CFLAGS} dH.cpp -o downloadHelperGUI

remove:
	rm downloadHelperCLI downloadHelperGUI
