# Compiler and flags
MACOS_SDL_CFLAGS = -I/opt/homebrew/Cellar/sdl2/2.32.8/include -D_THREAD_SAFE
LINUX_SDL_CFLAGS = $(shell sdl2-config --cflags)
WINDOWS_SDL_CFLAGS = -IC:/SDL2/include

MACOS_LDLIBS = $(shell sdl2-config --libs) -lm
LINUX_LDLIBS = $(shell sdl2-config --libs) -lm
WINDOWS_LDLIBS = -LC:/SDL2/lib -lSDL2main -lSDL2 -mwindows

CC_macos = gcc
CC_linux = gcc
CC_windows = x86_64-w64-mingw32-gcc

RUNTIME ?= macos

ifeq ($(RUNTIME),macos)
    CC := $(CC_macos)
    CFLAGS := -Wall -Wextra -std=c99 -Iinclude $(MACOS_SDL_CFLAGS)
    LDLIBS := $(MACOS_LDLIBS)
    EXT :=
else ifeq ($(RUNTIME),linux)
    CC := $(CC_linux)
    CFLAGS := -Wall -Wextra -std=c99 -Iinclude $(LINUX_SDL_CFLAGS)
    LDLIBS := $(LINUX_LDLIBS)
    EXT :=
else ifeq ($(RUNTIME),windows)
    CC := $(CC_windows)
    CFLAGS := -Wall -Wextra -std=c99 -Iinclude $(WINDOWS_SDL_CFLAGS)
    LDLIBS := $(WINDOWS_LDLIBS)
    EXT := .exe
else
    $(error Unknown RUNTIME: $(RUNTIME))
endif

SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

TARGET = $(BUILD_DIR)/3drender$(EXT)
PUBLISH_DIR = publish
PUBLISH_BIN = $(PUBLISH_DIR)/3drender$(EXT)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(OBJS) -o $@ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(PUBLISH_DIR)

run: all
	./$(TARGET)

publish: $(OBJS)
	@mkdir -p $(PUBLISH_DIR)
	$(CC) $(OBJS) -o $(PUBLISH_BIN) $(LDLIBS)
	@echo "Published binary to $(PUBLISH_BIN)"

.PHONY: all clean run publish
