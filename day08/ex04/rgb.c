#include "main.h"

void	init_timer0()
{
	// prescaler -> doc : 16.11.2
	SET(TCCR0B, CS02); // set prescaling to F_CPU/256 -> 62500 bit 1
	RESET(TCCR0B, CS01); // set prescaling to F_CPU/256 -> 62500 bit 2
	SET(TCCR0B, CS00); // set prescaling to F_CPU/256 -> 62500 bit 3

	SET(TCCR0A, COM0A1); // clear OC0A on compare, set a BOTTOM
	RESET(TCCR0A, COM0A0);

	SET(TCCR0A, COM0B1); // clear OC0B on compare, set a BOTTOM
	RESET(TCCR0A, COM0B0);

	// set counter mode operation to PWM
	RESET(TCCR0B, WGM02); // set bit 0
	SET(TCCR0A, WGM01); // set bit 1
	SET(TCCR0A, WGM00); // reset bit 0

	OCR0A = 255;
	OCR0B = 255;
}

void	init_timer2()
{
	// prescaler -> doc : 16.11.2
	SET(TCCR2B, CS22); // set prescaling to F_CPU/256 -> 62500 bit 1
	SET(TCCR2B, CS21); // set prescaling to F_CPU/256 -> 62500 bit 2
	RESET(TCCR2B, CS20); // set prescaling to F_CPU/256 -> 62500 bit 3

	SET(TCCR2A, COM2B1); // clear OC2B on compare, set a BOTTOM
	RESET(TCCR2A, COM2B0);

	// set counter mode operation to PWM
	RESET(TCCR2B, WGM22); // set bit 0
	SET(TCCR2A, WGM21); // set bit 1
	SET(TCCR2A, WGM20); // reset bit 0

	OCR2A = 255;
	OCR2B = 255;
}

void	init_timer1()
{
	// prescaler -> doc : 16.11.2
	SET(TCCR1B, CS12); // set prescaling to F_CPU/256 -> 62500 bit 1
	RESET(TCCR1B, CS11); // set prescaling to F_CPU/256 -> 62500 bit 2
	RESET(TCCR1B, CS10); // set prescaling to F_CPU/256 -> 62500 bit 3

	SET(TIMSK1, OCIE1A); // set interrupt

	SET(TCCR1A, COM1B1); // clear OC1B on compare, set a BOTTOM
	RESET(TCCR1A, COM1B0);

	// set counter mode operation to PWM
	RESET(TCCR1B, WGM13); // set bit 0
	SET(TCCR1B, WGM12); // set bit 0
	RESET(TCCR1A, WGM11); // set bit 1
	RESET(TCCR1A, WGM10); // reset bit 0

	OCR1A = 1562;
}

ISR(TIMER1_COMPA_vect)
{
	static uint8_t i = 0;

	if (rainbowing == true)
	{
		wheel(i);
		i++;
	}
}

void	init_rgb()
{
	init_timer0();
	init_timer1();
	init_timer2();
}

void	set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	uint32_t	val = 0;
	val |= r;
	val <<= 8;
	val |= g;
	val <<= 8;
	val |= b;
	write_all_leds(val, val, val);
	OCR0B = r;
	OCR0A = g;
	OCR2B = b;
}

void wheel(uint8_t pos) {
	pos = 255 - pos;
	if (pos < 85) {
		set_rgb(255 - pos * 3, 0, pos * 3);
	} else if (pos < 170) {
	pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	} else {
	pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
}
}