# Makefile for random_to_bitmap by Thomas Chevalier
# Copyright 2017 - All rights reserved
# thomasom [dot] chevalier [at] gmail [dot] com

OPTIMIZATION = s

CXX = g++ -O$(OPTIMIZATION) -s -flto -std=c++11
WFLAG = -Werror -pedantic -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wshadow -Weffc++ -Wredundant-decls -Wdouble-promotion -Winit-self -Wswitch-default -Wswitch-enum -Wundef -Wlogical-op -Winline
CXXFLAGS = $(WFLAG)
LDFLAGS = 

SDIR = src

SRC = $(wildcard $(SDIR)/*.cpp)
OBJS = $(SRC:.cpp=.o)
TARGET=random_to_bitmap

all: $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $^

main.o: Bitmap.h Pixel.h FileHeader.h ImageHeader.h
Pixel.o: Pixel.h
Bitmap.o: Bitmap.h

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	@rm $(SDIR)/*.o

clean-all: clean
	@rm $(TARGET)

rebuild: clean all
