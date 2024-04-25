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
	write_spi(0b111 << 5 | (intensity & 0b11111));
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

void	set_led_with_adc(uint8_t val)
{
	write_all_leds(0x01FF0000 * (val >= 255 * 0.33),
		0x01FF0000 * (val >= 255 * 0.66),
		0x01FF0000 * (val >= 255 * 1));
}

int	main()
{
	SET(DDRB, SS); // output
	SET(DDRB, MOSI); // output
	SET(DDRB, SCK); // output
	init_spi();
	init_ADC();
	while (1)
	{
		SET(ADCSRA, ADSC);
		while ((ADCSRA & (1 << ADSC)) != 0)
			;
		set_led_with_adc(ADCH);
		_delay_ms(20);
	}
}
