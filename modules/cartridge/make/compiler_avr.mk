# Name of the Microcontroller
MCU := $(DEVICE)

# Clock of the Microcontroller (Hz)
F_CPU := 16000000

# Fuses
L_FUSE=0xFF
H_FUSE=0xDA
E_FUSE=0xFD

# Progammer
# [usbasp|arduino]
PROGRAMMER=arduino
PORT=-P/dev/ttyUSB0


EXTRA_FLAGS += -mmcu=$(MCU)
EXTRA_FLAGS += -DF_CPU=$(F_CPU)UL
################################################################################
# Compiler
################################################################################
CC := avr-gcc

CFLAGS += -c                       # Compile and assemble, but do not link
CFLAGS += -Os                      # Optimize for space rather than speed
CFLAGS += -std=c99                 # Use this C standard

#CFLAGS += -Werror                  # Treat all warnings as errors
CFLAGS += -Wall                    # Print all warnings
CFLAGS += -Wextra                  # Print extra (possibly unwanted) warnings
CFLAGS += -Wunused                 # Enable all -Wunused- warnings
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
LD := avr-gcc

LDFLAGS = 


build: hex

hex: $(TARGET).hex size

eeprom: $(TARGET)_eeprom.hex

$(TARGET).hex: $(TARGET).elf
	@echo "\n\n--- building .hex file ---"
	avr-objcopy -O ihex -j .data -j .text $(BUILDPATH)/$(TARGET).elf $(BUILDPATH)/$(TARGET).hex
	@echo "-> $(BUILDPATH)/$(TARGET).hex"

$(TARGET)_eeprom.hex: $(TARGET).elf
	@echo "\n\n--- building eeprom ---"
	avr-objcopy -O ihex -j .eeprom --change-section-lma .eeprom=1 $(BUILDPATH)/$(TARGET).elf $(BUILDPATH)/$(TARGET)_eeprom.hex

$(TARGET).elf: $(OBJECTS)
	@echo "\n\n--- building .elf file ---"
	$(LD)  $(LDFLAGS) $(EXTRA_FLAGS) $(OBJECTS) -o $(BUILDPATH)/$(TARGET).elf
	@echo "-> $(BUILDPATH)/$(TARGET).elf"

size:
	@echo "\n\n--- calculating size ---"
	avr-size --mcu=$(MCU) -C $(BUILDPATH)/$(TARGET).elf

flash_hex:
	avrdude -p $(MCU) $(PORT) $(BAUD) -c $(PROGRAMMER) -U flash:w:$(BUILDPATH)/$(TARGET).hex:a
	#	micronucleus --run $(BUILDPATH)/$(TARGET).hex

read_fuses:
	avrdude -p $(MCU) $(PORT) $(BAUD) -c $(PROGRAMMER) -U lfuse:r:-:i

flash_fuses:
	avrdude -p $(MCU) $(PORT) $(BAUD) -c $(PROGRAMMER) -U lfuse:w:$(L_FUSE):m -U hfuse:w:$(H_FUSE):m -U efuse:w:$(E_FUSE):m
