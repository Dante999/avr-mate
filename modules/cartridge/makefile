################################################################################
# General project settings
################################################################################

# Name of the target (files will be named like that)
TARGET = avr-mate-cartridge

# Path where objects files and binaries will be located after creation
BUILDPATH = ./build

################################################################################
# Project files
################################################################################
BASE_DIR = $(shell pwd) 

#AVR_MATE_CORE_ROOT = $(BASE_DIR)/../core
AVR_MATE_CORE_ROOT = ../core



VPATH += src
VPATH += $(AVR_MATE_CORE_ROOT)/src
#VPATH += games/hello_world

EXTRA_FLAGS += -Iinclude
EXTRA_FLAGS += -I$(AVR_MATE_CORE_ROOT)/include



# List of all sources (only source-files)
SOURCES += protocol.c
SOURCES += cartridge.c
#SOURCES += main.c


SOURCES += haw_cartridge_emulator.c
SOURCES += haw_logger_emulator.c
SOURCES += fifo.c



################################################################################
# Makefile targets
################################################################################
include make/patterns.mk
include make/compiler.mk

include games/$(GAME)/makefile

all: prepare clean build

prepare:
	mkdir -p $(BUILDPATH)
	@echo "BASE_DIR: $(BASE_DIR)"
	@echo "CC: $(CC)"
	@echo "LD: $(LD)"
	@echo "object files: $(OBJECTS)"
	@echo "test object files: $(TEST_OBJECTS)"

$(OBJ_PATTERN): %.c
	@echo "CC \t$<"
	@$(CC) $(CFLAGS) $< -o $@

build: $(OBJECTS)
	@echo "\n\n-----\n linking objects\n-----"
	@$(LD) $(LDFLAGS) $(OBJECTS) -o $(BUILDPATH)/$(TARGET)
	@echo "-> $(BUILDPATH)/$(TARGET)"

clean_tmp:
	rm -rf $(BUILDPATH)/*.o
	rm -rf $(BUILDPATH)/*.elf

clean:
	rm -rf $(BUILDPATH)/*.o
	@echo "\n-------\n"



