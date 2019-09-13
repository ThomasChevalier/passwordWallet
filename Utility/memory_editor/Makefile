# Makefile for memory_editor by Thomas Chevalier
# Copyright 2018 - All rights reserved
# thomasom.chevalier@gmail.com

# Install rpl before
# apt-get install rpl

OPTIMIZATION = 2
AES_DIR = ../../library/AES

CXX = g++ -O$(OPTIMIZATION) -s -std=c++11
WFLAG = -pedantic -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wshadow -Weffc++ -Wredundant-decls -Wdouble-promotion -Winit-self -Wswitch-default -Wswitch-enum -Wundef -Wlogical-op -Winline
CXXFLAGS = $(WFLAG) -I$(AES_DIR)
LDFLAGS = -I$(AES_DIR)

SDIR = src
DDIR = src/dependency

SRC_CPP = $(wildcard $(SDIR)/*.cpp) $(wildcard $(AES_DIR)/*.cpp)
SRC_C =  $(wildcard $(DDIR)/*.c) $(wildcard $(DDIR)/*/*.c)
OBJS = $(SRC_CPP:.cpp=.o) $(SRC_C:.c=.o)
TARGET=memory_editor

all: $(OBJS)
	@echo =========
	@echo COMPILING
	@echo =========
	$(CXX) $(LDFLAGS) -o $(TARGET) $^

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

%.o : %.c
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	@rm $(SDIR)/*.o

clean-all: clean
	@rm $(TARGET)

rebuild: clean init all

init:
	@echo =============================
	@echo Creating dependency directory
	@echo =============================
	mkdir -p -v src/dependency/Security
	mkdir -p -v src/dependency/Memory

	@echo =====================
	@echo Copying required file
	@echo =====================
	cp ../../Code/src/Security/Password_List.h src/dependency/Security/Password_List.h
	cp ../../Code/src/Security/Password_List.c src/dependency/Security/Password_List.c
	cp ../../Code/src/Security/Password.h src/dependency/Security/Password.h
	cp ../../Code/src/Security/Password.c src/dependency/Security/Password.c
	cp ../../Code/src/Memory/MemoryLayout.h src/dependency/Memory/MemoryLayout.h
	cp ../../Code/src/Globals.h src/dependency/Globals.h
	cp ../../Code/src/Globals.c src/dependency/Globals.c

	@echo ===================
	@echo Replacing inclusion
	@echo ===================
	rpl "../Hardware/Fram.h" "../../Fram.h" src/dependency/Security/* -q
	rpl "Hardware/Fram.h" "../Fram.h" src/dependency/Globals.c -q
	rpl '#include "Aes.h"' '#include "../../Aes_transition.h"' src/dependency/Security/Password.c -q
	rpl '#include "Random.h"' '#include "../../Random.h"' src/dependency/Security/Password.c -q
	rpl '#include "../Graphics/ProgressBar.h"' " " src/dependency/Security/Password.c -q
	rpl "progress_add(1);" " " src/dependency/Security/Password.c -q
	rpl "progress_add(5);" " " src/dependency/Security/Password.c -q

	@echo ===================
	@echo Fixing Globals.[ch]
	@echo ===================
	rpl "#define GLOBALS_EVENTS GPIOR0" "extern uint8_t GLOBALS_EVENTS;" src/dependency/Globals.h -q
	rpl "#define OPTIONS_FLAG GPIOR1" "extern uint8_t OPTIONS_FLAG;" src/dependency/Globals.h -q
	echo "\nuint8_t GLOBALS_EVENTS = 0;" >> src/dependency/Globals.c
	echo "\nuint8_t OPTIONS_FLAG = 0;" >> src/dependency/Globals.c
	rpl '#include "Hardware/Led.h"' " " src/dependency/Globals.c -q
	rpl '#include <avr/io.h>' " " src/dependency/Globals.c -q
	rpl '#include <util/delay.h>' " " src/dependency/Globals.c -q
	rpl '#include <avr/io.h>' " " src/dependency/Globals.h -q
	rpl "_delay_ms(1);" " " src/dependency/Globals.c -q
