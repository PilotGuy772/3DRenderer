# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude



# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Find all .c files and convert to .o in build dir
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Output binary name
TARGET = $(BUILD_DIR)/3drender

# Default rule
all: $(TARGET)

# Link object files to create the final binary
$(TARGET): $(OBJS)
	echo $(CC) 
	echo $(CLFAGS)
	echo $(/opt/homebrew/bin/sdl2-config --cflags --libs)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(OBJS) -o $@

# Compile each .c file into an .o file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(BUILD_DIR)

# Optional: Run the program
run: all
	./$(TARGET)

.PHONY: all clean run
