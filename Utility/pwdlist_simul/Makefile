# Makefile for pwdlist_simul by Thomas Chevalier
# Copyright 2017 - All rights reserved
# thomasom [dot] chevalier [at] gmail [dot] com

OPTIMIZATION = s

CXX = g++ -O$(OPTIMIZATION) -s -flto -std=c++11
WFLAG = -Werror -pedantic -Wall -Wextra -Woverloaded-virtual -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-align -Wshadow -Weffc++ -Wredundant-decls -Wdouble-promotion -Winit-self -Wswitch-default -Wswitch-enum -Wundef -Wlogical-op -Winline
CXXFLAGS = $(WFLAG)
LDFLAGS =

SDIR = src

SRC = $(wildcard $(SDIR)/*.cpp) $(wildcard $(SDIR)/*.c)
_OBJS = $(SRC:.cpp=.c)
OBJS = $(_OBJS:.c=.o)
TARGET=pwdlist_simul

all: $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $^

#main.o: Application.h
#Application.o: Application.h Command.h Info.h Password.h Globals.h
#Command.o: Command.h
#FileHelper.o: FileHelper.h
#Info.o: Info.h FileHelper.h
#Password.o: Password.h FileHelper.h Globals.h

%.o : %.cpp
	echo $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	@rm $(SDIR)/*.o

clean-all: clean
	@rm $(TARGET)

rebuild: clean all