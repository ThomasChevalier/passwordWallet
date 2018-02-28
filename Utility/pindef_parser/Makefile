# Makefile for pindef_parser by Thomas Chevalier
# Copyright 2017 - All rights reserved
# thomasom [dot] chevalier [at] gmail [dot] com

OPTIMIZATION = 2

CXX =g++-6 -O$(OPTIMIZATION) -s -flto -std=c++11
WFLAG = -pedantic -Wall -Wextra -Woverloaded-virtual -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-align -Wshadow -Wredundant-decls -Wdouble-promotion -Winit-self -Wswitch-default -Wswitch-enum -Wundef -Wlogical-op -Winline
CXXFLAGS = $(WFLAG)
LDFLAGS =

SDIR = src

SRC = $(wildcard $(SDIR)/*.cpp)
OBJS = $(SRC:.cpp=.o)
TARGET=pindef_parser

all: $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $^

%.o : %.cpp
	echo $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	@rm $(SDIR)/*.o

clean-all: clean
	@rm $(TARGET)

rebuild: clean all
