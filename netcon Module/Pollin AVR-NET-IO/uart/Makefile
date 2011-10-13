
MCU = atmega32
F_CPU = 16000000


TOOLCHAIN_PATH = /cygdrive/C/Program\ Files/Atmel/AVR\ Tools/AVR\ Toolchain
BIN_PATH = $(TOOLCHAIN_PATH)/bin
CC = $(BIN_PATH)/avr-gcc
OBJCOPY = $(BIN_PATH)/avr-objcopy
SIZE = $(BIN_PATH)/avr-size

all:
	mkdir bin
	$(CC) *.c -mmcu=$(MCU) -std=c99 -Os -DF_CPU=$(F_CPU) -o ./bin/prog.elf
	$(OBJCOPY) -j .text -j .data -O ihex ./bin/prog.elf ./bin/prog.hex
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex ./bin/prog.elf ./bin/eprom.hex
	$(SIZE) -C --mcu=$(MCU) ./bin/prog.elf

