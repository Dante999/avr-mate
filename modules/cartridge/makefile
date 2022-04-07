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
UNITY_ROOT = extern/Unity
AVR_MATE_CORE_ROOT = ../core



VPATH += src
VPATH += $(AVR_MATE_CORE_ROOT)/src
VPATH += games/hello_world

EXTRA_FLAGS += -Iinclude
EXTRA_FLAGS += -I$(AVR_MATE_CORE_ROOT)/include



# List of all sources (only source-files)
SOURCES += protocol.c
SOURCES += cartridge.c
SOURCES += main.c


SOURCES += haw_cartridge_emulator.c
SOURCES += haw_logger_emulator.c
SOURCES += fifo.c



#TEST_SOURCES  = unity.c
#TEST_SOURCES += unity_fixture.c
#TEST_SOURCES += unity_memory.c

#TEST_SOURCES += main.c
#TEST_SOURCES += test_graphx.c
#TEST_SOURCES += test_protocol.c
#TEST_SOURCES += test_handheld_cartridge.c


################################################################################
# Building Object files 
################################################################################

# Pattern of the objects files 
OBJ_PATTERN = $(BUILDPATH)/%.o

# Change file extension from .c to .o
TMP1 =  $(SOURCES:%.c=$(OBJ_PATTERN))

# Change file extension from .cpp to .o
TMP2 = $(TMP1:%.cpp=$(OBJ_PATTERN))

# Final list ob object files to generate
OBJECTS = $(TMP2)

# Pattern of the objects files 
TEST_OBJ_PATTERN = $(BUILDPATH)/%.o

# Change file extension from .c to .o
TEST_OBJECTS = $(SOURCES:%.c=$(TEST_OBJ_PATTERN))

# Change file extension from .cpp to .o
TEST_OBJECTS += $(TEST_SOURCES:%.c=$(TEST_OBJ_PATTERN))


################################################################################
# Compiler
################################################################################
CC := gcc

CFLAGS = -I.                       # Include current file location to search path
#CFLAGS += -Iinclude
#CFLAGS += -I$(UNITY_ROOT)/src
#CFLAGS += -I$(UNITY_ROOT)/extras/fixture/src
#CFLAGS += -I$(UNITY_ROOT)/extras/memory/src

CFLAGS += -c                       # Compile and assemble, but do not link
CFLAGS += -g                       # Generate debug information in default format
CFLAGS += -Os                      # Optimize for space rather than speed
CFLAGS += -std=c99                 # Use this C standard

#CFLAGS += -Werror                  # Treat all warnings as errors
CFLAGS += -Wall                    # Print all warnings
CFLAGS += -Wextra                  # Print extra (possibly unwanted) warnings
#CFLAGS += -Wunused                 # Enable all -Wunused- warnings
CFLAGS += -Wshadow                 # Warn if a variable declaration shadows one from the parent context

CFLAGS += -fpack-struct            # Pack all structure members together without holes
CFLAGS += -fshort-enums            # Use the smallest needed integer for enum values
CFLAGS += -ffunction-sections      # Place each function their own section
CFLAGS += -fdata-sections          # Place data items into their own section
CFLAGS += -funsigned-char          # Make any unqualfied char type an unsigned char
CFLAGS += -funsigned-bitfields     # Make any unqualified bitfield type unsigned
CFLAGS += -DTEST
CFLAGS += $(EXTRA_FLAGS)

################################################################################
# Linker
################################################################################
LD := gcc

LDFLAGS = 

TEST_LDFLAGS += $(LDFLAGS)
TEST_LDFLAGS += -pthread
################################################################################
# Makefile targets
################################################################################

all: prepare clean test 
objects: $(OBJECTS)

prepare:
	mkdir -p $(BUILDPATH)
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

test: $(TEST_OBJECTS)
	@echo "\n\n-----\n linking test objects\n-----"
	$(LD) $(TEST_LDFLAGS) $(TEST_OBJECTS) -o $(BUILDPATH)/$(TARGET)_test

run_tests:
	@echo "\n\n-----\n RUNNING TESTS\n-----"
	@./build/$(TARGET)_test

clean_tmp:
	rm -rf $(BUILDPATH)/*.o
	rm -rf $(BUILDPATH)/*.elf

clean:
	rm -rf $(BUILDPATH)/*.o
	@echo "\n-------\n"
