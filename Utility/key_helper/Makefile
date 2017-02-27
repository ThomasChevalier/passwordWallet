# Makefile for key_helper by Thomas Chevalier
# Copyright 2017 - All rights reserved
# thomasom [dot] chevalier [at] gmail [dot] com

OPTIMIZATION = s
PWD_WALLET_SRC_DIR = ../../Code/src

CXX = g++ -O$(OPTIMIZATION) -s -flto -std=c++11
WFLAG = -Werror -pedantic -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wshadow -Weffc++ -Wredundant-decls -Wdouble-promotion -Winit-self -Wswitch-default -Wswitch-enum -Wundef -Wlogical-op -Winline
CXXFLAGS = $(WFLAG) -I$(PWD_WALLET_SRC_DIR)
LDFLAGS = -I$(PWD_WALLET_SRC_DIR)

SDIR = src

SRC = $(wildcard $(SDIR)/*.cpp)
OBJS = $(SRC:.cpp=.o)
TARGET=key_helper

all: $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $^

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	@rm $(SDIR)/*.o

clean-all: clean
	@rm $(TARGET)

rebuild: clean all