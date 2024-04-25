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
	for (uint8_t i = 0; i < 4; i++) {
		write_spi(0x0);
	}
}

void	send_end()
{
	for (uint8_t i = 0; i < 4; i++) {
		write_spi(0xff);
	}
}

void	send_rgb(uint8_t intensity, uint8_t r, uint8_t g, uint8_t b)
{
	write_spi(0b11100111);
	write_spi(b);
	write_spi(g);
	write_spi(r);
}

void	write_all_leds(uint32_t led1, uint32_t led2, uint32_t led3)
{
	send_start();
	send_rgb((led1 & 0xff000000) >> 24,
		(led1 & 0xff0000) >> 16, (led1 & 0xff00) >> 8, (led1 & 0xff));

	send_rgb((led2 & 0xff000000) >> 24,
		(led2 & 0xff0000) >> 16, (led2 & 0xff00) >> 8, (led2 & 0xff));

	send_rgb((led3 & 0xff000000) >> 24,
		(led3 & 0xff0000) >> 16, (led3 & 0xff00) >> 8, (led3 & 0xff));
	send_end();
}

int	main()
{
	SET(DDRB, SS); // output
	SET(DDRB, MOSI); // output
	SET(DDRB, SCK); // output
	init_spi();
	while (1)
	{
		// red
		write_all_leds(0x0FFF0000, 0x0, 0x0);
		_delay_ms(1000);
		// green
		write_all_leds(0x0F00FF00, 0x0, 0x0);
		_delay_ms(1000);
		// blue
		write_all_leds(0x0F0000FF, 0x0, 0x0);
		_delay_ms(1000);
		// yellow
		write_all_leds(0x0FFFFF00, 0x0, 0x0);
		_delay_ms(1000);
		// cyan
		write_all_leds(0x0F00FFFF, 0x0, 0x0);
		_delay_ms(1000);
		// magenta
		write_all_leds(0x0FFF00FF, 0x0, 0x0);
		_delay_ms(1000);
		// white
		write_all_leds(0x0FFFFFFF, 0x0, 0x0);
		_delay_ms(1000);
	}
}
