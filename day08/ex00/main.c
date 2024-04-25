#include "main.h"

#define SS PB2
#define MOSI PB3
#define SCK PB5

void	init_spi()
{
	// enable SPI
	uint8_t tmp = 0;

	SET(tmp, MSTR);
	SET(tmp, SPE);
	// set clock rate
	// RESET(SPSR, SPI2X);
	// SET(tmp, SPR1);
	SET(tmp, SPR0);
	SPCR = tmp;
}

void	write_spi(uint8_t byte)
{
	SPDR = byte;
	while (!(SPSR & (1 << SPIF)))
		;
	// uart_printstr("success byte send\r\n");
}

void	send_start()
{
	for (uint8_t i = 0; i < 3; i++) {
		write_spi(0x0);
	}
}

void	send_end()
{
	for (uint8_t i = 0; i < 3; i++) {
		write_spi(0xff);
	}
}

void	send_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	write_spi(0b11100001);
	write_spi(b);
	write_spi(g);
	write_spi(r);
}

int	main()
{
	SET(DDRB, SS); // output
	SET(DDRB, MOSI); // output
	SET(DDRB, SCK); // output
	init_spi();
	send_start();
	send_rgb(0xff, 0, 0);
	send_end();

	while (1);
	
}
