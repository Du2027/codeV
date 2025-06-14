BUILD_DIR = build
BUILD = $(BUILD_DIR)/SchSchwGame.bin

CFLAGS = -I. -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -std=c++11 -pthread
CC = g++

all:$(BUILD)

$(BUILD_DIR):
	@echo "creating build directory"
	@mkdir -p $(BUILD_DIR)

# WM
$(BUILD_DIR)/windowManager.o: windowManager.cpp | $(BUILD_DIR)
	@echo "compiling WM"
	@${CC} ${CFLAGS} -c windowManager.cpp -o $(BUILD_DIR)/windowManager.o

# game
$(BUILD_DIR)/introScreen.o: introScreen.cpp | $(BUILD_DIR) # | for a prequisite that must be build beforehand
	@echo "compiling game"
	@${CC} ${CFLAGS} -c introScreen.cpp -o $(BUILD_DIR)/introScreen.o

# constants
$(BUILD_DIR)/constants.o: constants.cpp | $(BUILD_DIR)
	@echo "compiling constants"
	@${CC} ${CFLAGS} -c constants.cpp -o $(BUILD_DIR)/constants.o

# classes
$(BUILD_DIR)/classes.o: classes.cpp | $(BUILD_DIR)
	@echo "compiling classes"
	@${CC} ${CFLAGS} -c classes.cpp -o $(BUILD_DIR)/classes.o

# gameObjects
$(BUILD_DIR)/gameObjects.o: gameObjects.cpp | $(BUILD_DIR)
	@echo "compiling gameObjects"
	@${CC} ${CFLAGS} -c gameObjects.cpp -o $(BUILD_DIR)/gameObjects.o

# linking
$(BUILD): $(BUILD_DIR)/constants.o $(BUILD_DIR)/classes.o $(BUILD_DIR)/gameObjects.o $(BUILD_DIR)/introScreen.o $(BUILD_DIR)/windowManager.o | $(BUILD_DIR) 
	@echo "linking to final"
	@${CC} ${CFLAGS} $^ -o $@ # $^ for prequisites and $@ for target

clean:
	@echo "removing build"
	@rm -rf $(BUILD_DIR)
