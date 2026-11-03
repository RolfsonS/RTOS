MCU        = atmega2560
F_CPU      = 16000000UL
PORT       = /dev/ttyACM0
BAUD       = 115200
PROGRAMMER = wiring

CC      = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

CFLAGS  = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall

TARGET = blink
SRCS   = $(TARGET).c switch.s

all: $(TARGET).hex

$(TARGET).elf: $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS)

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(BAUD) -D -U flash:w:$<:i

clean:
	rm -f $(TARGET).elf $(TARGET).hex

.PHONY: all flash clean
