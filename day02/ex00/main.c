#include "main.h"

volatile uint8_t	is_pressed = 0;

ISR(INT0_vect)
{
	is_pressed = 1;
	RESET(EIMSK, INT0);
	TOGGLE(PORTB, PB0);
}

int main()
{
	RESET(DDRD, PD2);
	SET(DDRB, PB0);
	SET(DDRB, PB1);
	SET(SREG, 7);	// activate global interrupt

	SET(EICRA, ISC01);		// set interrupt as falling edge -> doc : 13.2.1
	RESET(EICRA, ISC00);	// set interrupt as falling edge -> doc : 13.2.1

	SET(EIMSK, INT0); // set external pin interrupt enable

	while (1)
	{
		if (is_pressed == 1)
		{
			TOGGLE(PORTB, PB1);
			
			_delay_ms(500);
			is_pressed = 0;
			SET(EIMSK, INT0);
		}
	}
}