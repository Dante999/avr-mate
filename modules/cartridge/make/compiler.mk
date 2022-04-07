
################################################################################
# Compiler
################################################################################
CC := gcc

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
CFLAGS += $(EXTRA_FLAGS)

################################################################################
# Linker
################################################################################
LD := gcc

LDFLAGS = 

TEST_LDFLAGS += $(LDFLAGS)
