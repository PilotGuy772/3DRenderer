# Compiler and flags
MACOS_SDL_CFLAGS = -I/opt/homebrew/Cellar/sdl2/2.32.8/include -D_THREAD_SAFE
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude $(MACOS_SDL_CFLAGS) # shell sdl2-config --cflags
LDLIBS = $(shell sdl2-config --libs) -lm

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Find all .c files and convert to .o in build dir
SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Output binary name
TARGET = $(BUILD_DIR)/3drender

# Default rule
all: $(TARGET)

# Link object files to create the final binary
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(OBJS) -o $@ $(LDLIBS)

# Compile each .c file into an .o file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(BUILD_DIR)

# Optional: Run the program
run: all
	./$(TARGET)

.PHONY: all clean run
