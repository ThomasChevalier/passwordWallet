
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
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -I$(CDIR)/ -flto=4 -s -fdata-sections -Werror -mcall-prologues
LD_FLAGS     = -Os -flto=4 -s

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
