CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# convert all .c files into .o files in build dir
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# output binary
TARGET = $(BUILD_DIR)/3drender

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(OBJS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run: all
	./$(TARGET)

.PHONY: all clean run