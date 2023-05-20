BULID_DIR = $(shell pwd)
LIB_DIR := lib
CC := g++


CFLAG := -g 
CFLAG += -std=c++17
CFLAG += -Wall
CFLAG += -O2


SRC := $(wildcard $(LIB_DIR)/*.cpp)
OBJ := $(patsubst $(LIB_DIR)/%.cpp, $(LIB_DIR)/%.o, $(SRC))

all: $(OBJ)
	$(CC) $(CFLAG) main.cpp $(OBJ)

dev: all
	./a.out

# a.out: $(OBJ)
# 	$(CC) $(CFLAG) main.cpp $(OBJ)

$(LIB_DIR)/%.o: $(LIB_DIR)/%.cpp
	$(CC) -I $(LIB_DIR) -c $< $(CFLAG) -o $@

clean:
	rm -f *.out $(LIB_DIR)/*.o
