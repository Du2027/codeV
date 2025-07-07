CC = g++
CFLAGS = -I. -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -std=c++11 -pthread

BUILD_DIR = build
BUILD = $(BUILD_DIR)/PoHM-helper.bin

all:$(BUILD)

$(BUILD_DIR):
	@echo "creating build directory"
	@mkdir -p $(BUILD_DIR)

# all
$(BUILD): src/main.cpp src/errorMessage.cpp | $(BUILD_DIR) 
	@echo "compiling & linking to final"
	@${CC} ${CFLAGS} $^ -o $@

clean:
	@echo "removing build"
	@rm -rf $(BUILD_DIR)