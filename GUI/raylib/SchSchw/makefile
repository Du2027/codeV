BUILD_DIR = build
BUILD = $(BUILD_DIR)/SchSchwGame.bin

CFLAGS = -I. -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -std=c++11 -pthread
CC = g++

all:$(BUILD)

$(BUILD_DIR):
	@echo "creating build directory"
	@mkdir -p $(BUILD_DIR)

# constants
$(BUILD_DIR)/constants.o: constants.cpp | $(BUILD_DIR)
	@echo "compiling constants"
	@${CC} ${CFLAGS} -c constants.cpp -o $(BUILD_DIR)/constants.o

# classes
$(BUILD_DIR)/classes.o: classes.cpp | $(BUILD_DIR)
	@echo "compiling classes"
	@${CC} ${CFLAGS} -c classes.cpp -o $(BUILD_DIR)/classes.o



# WM
$(BUILD_DIR)/windowManager.o: windowManager.cpp | $(BUILD_DIR)
	@echo "compiling WM"
	@${CC} ${CFLAGS} -c windowManager.cpp -o $(BUILD_DIR)/windowManager.o

# IntroScreen
$(BUILD_DIR)/introScreen.o: introScreen.cpp | $(BUILD_DIR) # | for a prequisite that must be build beforehand
	@echo "compiling introScreen"
	@${CC} ${CFLAGS} -c introScreen.cpp -o $(BUILD_DIR)/introScreen.o

# SettingsScreen
$(BUILD_DIR)/settingsScreen.o: settingsScreen.cpp | $(BUILD_DIR)
	@echo "compiling settingsScreen"
	@${CC} ${CFLAGS} -c settingsScreen.cpp -o $(BUILD_DIR)/settingsScreen.o

# GameScreen
$(BUILD_DIR)/gameScreen.o: gameScreen.cpp | $(BUILD_DIR)
	@echo "compiling gameScreen"
	@${CC} ${CFLAGS} -c gameScreen.cpp -o $(BUILD_DIR)/gameScreen.o

# GameScreen
$(BUILD_DIR)/errorScreen.o: errorScreen.cpp | $(BUILD_DIR)
	@echo "compiling errorScreen"
	@${CC} ${CFLAGS} -c errorScreen.cpp -o $(BUILD_DIR)/errorScreen.o



# linking
$(BUILD): $(BUILD_DIR)/constants.o $(BUILD_DIR)/classes.o $(BUILD_DIR)/introScreen.o $(BUILD_DIR)/windowManager.o $(BUILD_DIR)/settingsScreen.o $(BUILD_DIR)/gameScreen.o  $(BUILD_DIR)/errorScreen.o | $(BUILD_DIR) 
	@echo "linking to final"
	@${CC} ${CFLAGS} $^ -o $@ # $^ for prequisites and $@ for target

clean:
	@echo "removing build"
	@rm -rf $(BUILD_DIR)
