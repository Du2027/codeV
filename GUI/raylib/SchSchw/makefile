BUILD_DIR = build
BUILD = $(BUILD_DIR)/SchSchwGame.bin

CFLAGS = -I. -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -std=c++11 -pthread
CC = g++

all:$(BUILD)

$(BUILD_DIR):
	@echo "creating build directory"
	@mkdir -p $(BUILD_DIR)

# game
$(BUILD_DIR)/game.o: game.cpp | $(BUILD_DIR) # | for a prequisite that must be build beforehand
	@echo "compiling game"
	@${CC} ${CFLAGS} -c game.cpp -o $(BUILD_DIR)/game.o

# constants
$(BUILD_DIR)/constants.o: constants.cpp | $(BUILD_DIR)
	@echo "compiling constants"
	@${CC} ${CFLAGS} -c constants.cpp -o $(BUILD_DIR)/constants.o

# waterObjects
$(BUILD_DIR)/waterObject.o: WaterObject.cpp | $(BUILD_DIR)
	@echo "compiling waterObj"
	@${CC} ${CFLAGS} -c WaterObject.cpp -o $(BUILD_DIR)/waterObject.o

# droplets
$(BUILD_DIR)/droplet.o: Droplet.cpp | $(BUILD_DIR)
	@echo "compiling droplet"
	@${CC} ${CFLAGS} -c Droplet.cpp -o $(BUILD_DIR)/droplet.o

# linking
$(BUILD): $(BUILD_DIR)/game.o $(BUILD_DIR)/waterObject.o $(BUILD_DIR)/droplet.o $(BUILD_DIR)/constants.o | $(BUILD_DIR) 
	@echo "linking to final"
	@${CC} ${CFLAGS} $^ -o $@ # $^ for prequisites and $@ for target

clean:
	@echo "removing build"
	@rm -rf $(BUILD_DIR)
