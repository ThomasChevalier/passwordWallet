# Makefile for memory_parser by Thomas Chevalier
# Copyright 2017 - All rights reserved
# thomasom.chevalier@gmail.com

OPTIMIZATION = 2
PUGI_XML_DIR = ../../library/pugixml-1.8/src
AES_DIR = ../../library/AES

CXX = g++ -O$(OPTIMIZATION) -s -flto -std=c++11
WFLAG = -pedantic -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wshadow -Weffc++ -Wredundant-decls -Wdouble-promotion -Winit-self -Wswitch-default -Wswitch-enum -Wundef -Wlogical-op -Winline
CXXFLAGS = $(WFLAG) -I$(PUGI_XML_DIR) -I$(AES_DIR)
LDFLAGS = -I$(PUGI_XML_DIR) -I$(AES_DIR)

SDIR = .

SRC = $(wildcard $(SDIR)/*.cpp) $(wildcard $(PUGI_XML_DIR)/*.cpp) $(wildcard $(AES_DIR)/*.cpp)
OBJS = $(SRC:.cpp=.o)
TARGET=memory_parser

all: $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $^

pugixml.o : pugixml.hpp

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	@rm $(SDIR)/*.o

clean-all: clean
	@rm $(TARGET)

rebuild: clean all