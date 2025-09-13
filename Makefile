# Makefile for Notepad++--
# Author: [ywbrian]
# Date: [2025-09-13]

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
LDFLAGS = -luser32 -lkernel32 -lgdi32

# Build config
BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = include
TARGET = Notepad++--

.PHONY: all debug release rebuild clean

DEBUG_FLAGS = -g -O0 -DDEBUG
RELEASE_FLAGS = -O2 -DNDEBUG

CONFIG ?= debug
ifeq ($(CONFIG), release)
    CXXFLAGS += $(RELEASE_FLAGS)
    BUILD_DIR := $(BUILD_DIR)/release
else
    CXXFLAGS += $(DEBUG_FLAGS)
    BUILD_DIR := $(BUILD_DIR)/debug
endif

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

CXXFLAGS += -I$(INCLUDE_DIR)

# Default target
all: $(BUILD_DIR)/$(TARGET).exe

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Build exe
$(BUILD_DIR)/$(TARGET).exe: $(OBJECTS) | $(BUILD_DIR)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

debug:
	$(MAKE) CONFIG=debug

release:
	$(MAKE) CONFIG=release

clean:
	@if [ -d "build" ]; then rm -rf build; fi

rebuild: clean all