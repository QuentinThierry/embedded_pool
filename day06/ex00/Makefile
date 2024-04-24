CC=avr-gcc
BAUDRATE=115200
F_CPU=16000000
PORT=/dev/ttyUSB0
CFLAGS=-mmcu=atmega328p -DF_CPU=$(F_CPU) -DUART_BAUDRATE=$(BAUDRATE) -Os
NAME=main
SRC=	\
	main.c

OBJ=$(SRC:.c=.o)

%.o:%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

all: hex flash

hex: $(NAME).hex

$(NAME).bin: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME).bin

$(NAME).hex: $(NAME).bin
	avr-objcopy -O ihex $(NAME).bin $(NAME).hex

flash: hex
	avrdude -p m328p -c arduino -b $(BAUDRATE) -e -u -U flash:w:$(NAME).hex:i -P$(PORT) $(NAME).hex

clean:
	rm -rf $(NAME).bin $(NAME).hex $(OBJ)

.PHONY: all hex flash clean