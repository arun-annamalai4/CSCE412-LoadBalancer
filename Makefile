CXX := clang++
SDK_CXX_INCLUDE := /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1

CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -Iinclude -isystem $(SDK_CXX_INCLUDE)
LDFLAGS :=

SRC_DIR := src
OBJ_DIR := build/obj
BIN_DIR := build/bin
TARGET := $(BIN_DIR)/load_balancer

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

.PHONY: all check app clean

# Default target: compile all translation units.
all: check

check: $(OBJECTS)


app: $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf build
