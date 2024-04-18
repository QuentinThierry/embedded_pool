#include "main.h"

ISR(INT0_vect)
{
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

	while (1);
}