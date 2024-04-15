CC=avr-gcc
CFLAGS=-mmcu=atmega328p -DF_CPU=16000000

all: hex flash

hex:
	$(CC) $(CFLAGS) -Os main.c -o main.bin
	avr-objcopy -O ihex main.bin main.hex

flash:
	avrdude -p m328p -c arduino -b 115200 -e -u -U flash:w:main.hex:i -P/dev/ttyUSB0 main.hex

clean:
	rm -rf main.bin main.hex