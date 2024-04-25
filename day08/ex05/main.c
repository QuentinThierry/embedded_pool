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
	write_spi(0b111 << 5 | (1 & 0b11111));
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

#define R 0
#define G 1
#define B 2

void	set_led_with_adc(uint8_t val, uint8_t led_num, uint8_t rgb)
{
	static uint32_t leds[3] = {0};

	uint32_t	led_rgb = 0;
	uint32_t	val_cpy = val;

	switch (led_num) {
		case 0:
			led_rgb = leds[0];
			if (rgb == R)
				led_rgb &= 0xff00ffff, led_rgb += (val_cpy << 16);
			if (rgb == G)
				led_rgb &= 0xffff00ff, led_rgb += (val_cpy << 8);
			if (rgb == B)
				led_rgb &= 0xffffff00, led_rgb += val_cpy;
			write_all_leds(led_rgb, leds[1], leds[2]);
			leds[0] = led_rgb;
			break;
		case 1:
			led_rgb = leds[1];
			if (rgb == R)
				led_rgb &= 0xff00ffff, led_rgb += (val_cpy << 16);
			if (rgb == G)
				led_rgb &= 0xffff00ff, led_rgb += (val_cpy << 8);
			if (rgb == B)
				led_rgb &= 0xffffff00, led_rgb += val_cpy;
			write_all_leds(leds[0], led_rgb, leds[2]);
			leds[1] = led_rgb;
			break;
		case 2:
			led_rgb = leds[2];
			if (rgb == R)
				led_rgb &= 0xff00ffff, led_rgb += (val_cpy << 16);
			if (rgb == G)
				led_rgb &= 0xffff00ff, led_rgb += (val_cpy << 8);
			if (rgb == B)
				led_rgb &= 0xffffff00, led_rgb += val_cpy;
			write_all_leds(leds[0], leds[1], led_rgb);
			leds[2] = led_rgb;
			break;
	}
}

int	main()
{
	SET(DDRB, SS); // output
	SET(DDRB, MOSI); // output
	SET(DDRB, SCK); // output
	init_spi();
	init_ADC();
	uint8_t led_num = 0;
	uint8_t rgb = 0;
	while (1)
	{
		SET(ADCSRA, ADSC);
		while ((ADCSRA & (1 << ADSC)) != 0)
			;
		set_led_with_adc(ADCH, led_num, rgb);
		if ((PIND & (1 << PD2)) == 0)
		{
			rgb++;
			if (rgb == 3)
				rgb = 0;
			while ((PIND & (1 << PD2)) == 0);
		}
		if ((PIND & (1 << PD4)) == 0)
		{
			led_num++;
			if (led_num == 3)
				led_num = 0;
			while ((PIND & (1 << PD4)) == 0);
		}
		
		_delay_ms(100);
	}
}
