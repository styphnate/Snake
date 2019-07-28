SRC_DIR := src
OBJ_DIR := obj

CXXFLAGS += -std=c++17

CXXFLAGS += -Wall -Wextra -pedantic-errors

LDLIBS := $(shell pkg-config sfml-all --libs)
SRC_FILES := $(wildcard $(SRC_DIR)/*.cc)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cc,$(OBJ_DIR)/%.o,$(SRC_FILES))

all: $(OBJ_FILES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	g++ $(CXXFLAGS) -c -o $@ $<
	g++ -o out $(OBJ_FILES) $(LDLIBS)