
# Makefile for Password Wallet - Lufa Version
# All rights reserved - Thomas Chevalier - 2017

MCU          = atmega32u4
ARCH         = AVR8
BOARD        = NONE
F_CPU        = 8000000
F_USB        = $(F_CPU)
OPTIMIZATION = s
TARGET       = main
SDIR=src
CDIR=src/UsbConfig
SRC          = $(wildcard $(SDIR)/*.c) $(LUFA_SRC_USB) $(wildcard $(SDIR)/*/*.c) $(wildcard $(SDIR)/FSM/States/*.c)
LUFA_PATH    = ./LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -I$(CDIR)/ -flto=12 -fdata-sections -mcall-prologues
LD_FLAGS     = -Os -flto=12

# Default target
all:

# Include LUFA build script makefiles
include $(LUFA_PATH)/Build/lufa_core.mk
include $(LUFA_PATH)/Build/lufa_sources.mk
include $(LUFA_PATH)/Build/lufa_build.mk
include $(LUFA_PATH)/Build/lufa_cppcheck.mk
include $(LUFA_PATH)/Build/lufa_doxygen.mk
include $(LUFA_PATH)/Build/lufa_dfu.mk
include $(LUFA_PATH)/Build/lufa_hid.mk
include $(LUFA_PATH)/Build/lufa_avrdude.mk
include $(LUFA_PATH)/Build/lufa_atprogram.mk

string:
	@echo "Update strings ... "
	../Utility/string_parser/string_parser --file strings.xml
	mv String.h src/Graphics/String.h
pin:
	@echo "Update pins' definition ... "
	../Utility/pindef_parser/pindef_parser --file pindef.xml
	mv PinDefinition.h src/Hardware/PinDefinition.h
option:
	@echo "Update options ... "
	python3.5 ../Utility/option_parser/parser.py options.xml
	mv Options_List.h src/Options/Options_List.h
	mv Options_List.c src/Options/Options_List.c
	
