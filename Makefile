# AVR-GCC Compiler and flags
CC = avr-gcc
CFLAGS = \
	-mmcu=$(MCU) \
	-DF_CPU=16000000UL \
	-Werror \
	-Os
# LDFLAGS = -Wl,--gc-sections \

# AVRDUDE settings
BAUDRATE = 115200
PROGRAMMER = arduino
MCU = atmega328p

# Windows specific rules
ifeq ($(OS), Windows_NT)
# Windows specific rules
PORT = COM3
RM = del /Q
else
# Linux specific rules
PORT = /dev/ttyUSB0
RM = rm
endif

# Directories
SRC_DIR = src
LIB_DIR = lib
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# Executable
TARGET = $(BIN_DIR)/app.elf
HEX = $(BIN_DIR)/app.hex

all: $(TARGET) $(HEX)

$(TARGET): $(OBJ) $(LIBOBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

$(HEX): $(TARGET)
	avr-objcopy -O ihex -R .eeprom $(TARGET) $(HEX)

upload: $(HEX)
	avrdude -b$(BAUDRATE) -c$(PROGRAMMER) -P$(PORT) -p$(MCU) -D -v -V -U flash:w:$(HEX)

size: $(TARGET)
	avr-size --mcu=$(MCU) --format=avr $(TARGET)

clean:
	$(RM) $(OBJ_DIR)\*.o
	$(RM) $(BIN_DIR)\*

.PHONY: all clean upload size