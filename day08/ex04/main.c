#include "main.h"

#define SS PB2
#define MOSI PB3
#define SCK PB5

void	init_spi()
{
	SET(DDRB, SS); // output
	SET(DDRB, MOSI); // output
	SET(DDRB, SCK); // output

	// enable SPI
	uint8_t tmp = 0;

	SET(tmp, MSTR);
	SET(tmp, SPE);
	// set clock rate
	SET(tmp, SPR0);
	SPCR = tmp;
}

void	write_spi(uint8_t byte)
{
	SPDR = byte;
	while (!(SPSR & (1 << SPIF)))
		;
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
	(void)intensity;
	write_spi(0b111 << 5 | (3 & 0b11111));
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

void	parse_led(uint32_t val)
{
	if ((val & 0xff) == 6)
		write_all_leds(val >> 8, 0, 0);
	if ((val & 0xff) == 7)
		write_all_leds(0, val >> 8, 0);
	if ((val & 0xff) == 8)
		write_all_leds(0, 0, val >> 8);
}

volatile bool rainbowing = false;

void	display_full_rainbow()
{
	rainbowing = true;
}

int	main()
{
	init_spi();
	uart_init();
	init_rgb();

	SET(SREG, 7);

	bool	has_error = false;
	bool	is_full_rainbow = false;
	uint32_t	led_val = 0;

	uart_printstr("> ");
	uint8_t i = 0;
	while (1)
	{
		_delay_ms(20);
		led_val = parsing(&has_error, &is_full_rainbow);
		if (is_full_rainbow)
		{
			display_full_rainbow();
		}
		else if (!has_error)
		{
			rainbowing = false;
			parse_led(led_val);
		}
		else
			uart_printstr("Error");

		has_error = false;
		is_full_rainbow = false;
		uart_printstr("\r\n> ");
	}
}
