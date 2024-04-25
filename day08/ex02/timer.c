#include "main.h"

#define BLINK_SPEED 1000
#define NB_MATCH_ONE_CYCLE 77

volatile uint8_t g_light_num = 0;

void	init_timer1()
{
	// prescaler -> doc : 16.11.2
	SET(SREG, 7);

	SET(TCCR1B, CS12);
	RESET(TCCR1B, CS11);
	RESET(TCCR1B, CS10);

	SET(TIMSK1, OCIE1A); // set interrupt to happen on OCR1A match
	SET(TIMSK1, OCIE1B); // set interrupt to happen on OCR1B match

	// set counter mode operation to CTC top on ICR1
	SET(TCCR1B, WGM13);
	SET(TCCR1B, WGM12);
	RESET(TCCR1A, WGM11);
	RESET(TCCR1A, WGM10);

	OCR1B = 15625; // 250 ms, reset light
	OCR1A = 20833; // 333 ms, set other light on
	ICR1 = OCR1A; // top
}

ISR(TIMER1_COMPA_vect)
{
	write_all_leds(0, 0, 0);
}

ISR(TIMER1_COMPB_vect)
{
	if (g_light_num == 3)
		g_light_num = 0;
	if (g_light_num == 0)
		write_all_leds(0x0FFF0000, 0, 0);
	else if (g_light_num == 1)
		write_all_leds(0, 0x0FFF0000, 0);
	else if (g_light_num == 2)
		write_all_leds(0, 0, 0x0FFF0000);
	g_light_num++;
}
