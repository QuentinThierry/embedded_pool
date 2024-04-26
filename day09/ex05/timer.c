#include "main.h"

#define BLINK_SPEED 1000
#define NB_MATCH_ONE_CYCLE 77

void	init_timer1()
{
	// prescaler -> doc : 16.11.2
	SET(TCCR1B, CS12); // set prescaling to F_CPU/256 -> 62500 bit 1
	RESET(TCCR1B, CS11); // set prescaling to F_CPU/256 -> 62500 bit 2
	RESET(TCCR1B, CS10); // set prescaling to F_CPU/256 -> 62500 bit 3

	SET(TIMSK1, OCIE1A); // interrupt

	// compare match -> doc : 16.11.1 - table 16-1
	SET(TCCR1A, COM1A1); // set toggle OC1A on compare match bit 1
	RESET(TCCR1A, COM1A0); // set toggle OC1A on compare match bit 2

	// set counter mode operation -> doc : 16.11.1 - table 16-4
	RESET(TCCR1B, WGM13);
	SET(TCCR1B, WGM12);
	RESET(TCCR1A, WGM11);
	RESET(TCCR1A, WGM10);

	OCR1A = 62500;
}



ISR(TIMER1_COMPA_vect)
{
	g_value_cpt++;
	if (g_value_cpt == 999)
		g_value_cpt = 0;
}

void	init_interrupt()
{
	SET(SREG, 7);	// activate global interrupt
}
