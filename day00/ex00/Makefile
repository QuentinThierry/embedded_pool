CC=avr-gcc
CFLAGS=-mmcu=atmega328p -DF_CPU=16000000 -Os
NAME=main
SRC=	\
	main.c

OBJ=$(SRC:.c=.o)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

all: hex flash

hex: $(NAME).hex

$(NAME).hex: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME).bin
	avr-objcopy -O ihex $(NAME).bin $(NAME).hex

flash:
	avrdude -p m328p -c arduino -b 115200 -e -u -U flash:w:$(NAME).hex:i -P/dev/ttyUSB0 $(NAME).hex

clean:
	rm -rf $(NAME).bin $(NAME).hex $(OBJ)

.PHONY: all hex flash clean