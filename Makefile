CC := g++
CXXFLAGS := -std=c++17 -Wall -I./src/include
LDFLAGS := -L./src/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml audio

# Define source directory and build directory
SRC_DIR := ./coding
BUILD_DIR := ./build
BIN_DIR := .

# Automatically include all source files
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
EXECUTABLE := $(BIN_DIR)/main.exe


# Default target
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@echo "Linking..."
	@mkdir -p $(BIN_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

# Compile each source file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Creating build directory..."
	@mkdir -p $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Cleaning up..."
	rm -rf $(BUILD_DIR) $(EXECUTABLE)

.PHONY: all clean


