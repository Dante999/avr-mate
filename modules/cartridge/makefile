################################################################################
# General project settings
################################################################################
ifeq ($(GAME),)
$(warning "Warning: Variable GAME not set! (default=hello_world)")
GAME := hello_world
endif

ifeq ($(DEVICE),)
$(warning "Warning: Variable DEVICE not set! (default=emulator)")
DEVICE := emulator
endif


# Name of the target (files will be named like that)
TARGET = avr-mate-cartridge


CURRENT_DIR = $(shell pwd)

# Path where objects files and binaries will be located after creation
BUILDPATH = ./build

################################################################################
# Project files
################################################################################
AVR_MATE_CORE_ROOT = ../core



VPATH += src
VPATH += $(AVR_MATE_CORE_ROOT)/src


EXTRA_FLAGS += -Iinclude
EXTRA_FLAGS += -I$(AVR_MATE_CORE_ROOT)/include


# List of all sources (only source-files)
SOURCES += protocol.c
SOURCES += cartridge.c
SOURCES += haw_cartridge_$(DEVICE).c
SOURCES += haw_logger_$(DEVICE).c

# sources only for emulator
ifeq ($(DEVICE),emulator)
SOURCES += fifo.c
endif






################################################################################
# Makefile targets
################################################################################
include make/patterns.mk
include games/$(GAME)/makefile

all: prepare clean build

ifeq ($(DEVICE),emulator)
include make/compiler_gcc.mk
else
include make/compiler_avr.mk
endif



clean:
	@echo "\n\n--- clean ---"
	rm -rf $(BUILDPATH)/*.o
	@echo "--------------------\n"

prepare:
	@mkdir -p $(BUILDPATH)
	@echo "\n\n--- stats ---"
	@echo "Compiler: $(CC)"
	@echo "Linker  : $(LD)"
	@echo "Device  : $(DEVICE)"
	@echo "Game    : $(GAME)"
	@echo "--------------------\n"


$(OBJ_PATTERN): %.c
	@echo "CC \t$<"
	@$(CC) $(CFLAGS) $< -o $@

#build: $(OBJECTS)
#	@echo "\n\n--- linking objects ---"
#	@$(LD) $(LDFLAGS) $(OBJECTS) -o $(BUILDPATH)/$(TARGET)
#	@echo "-> $(BUILDPATH)/$(TARGET)"
#	@echo "--------------------\n"

