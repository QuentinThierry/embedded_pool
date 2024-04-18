#include "main.h"

// volatile uint8_t	is_pressed = 0;

#define BLINK_SPEED 1000
#define NB_MATCH_ONE_CYCLE 77

void	init_timer0()
{
	// prescaler -> doc : 16.11.2
	SET(TCCR0B, CS02); // set prescaling to F_CPU/256 -> 62500 bit 1
	RESET(TCCR0B, CS01); // set prescaling to F_CPU/256 -> 62500 bit 2
	SET(TCCR0B, CS00); // set prescaling to F_CPU/256 -> 62500 bit 3

	SET(TIMSK0, OCIE0A); // set interrupt to happen on OCR01 match

	// set counter mode operation to CTC -> doc : 16.11.1 - table 16-4
	RESET(TCCR0B, WGM02); // set bit 1
	SET(TCCR0A, WGM01); // set bit 1
	RESET(TCCR0A, WGM00); // reset bit 1

	OCR0A = NB_MATCH_ONE_CYCLE; // set match time at 155 / 2 -> doc : 15.7.2 equation
}


void	init_timer1()
{
	// prescaler -> doc : 16.11.2
	SET(TCCR1B, CS12); // set prescaling to F_CPU/256 -> 62500 bit 1
	RESET(TCCR1B, CS11); // set prescaling to F_CPU/256 -> 62500 bit 2
	RESET(TCCR1B, CS10); // set prescaling to F_CPU/256 -> 62500 bit 3

	// compare match -> doc : 16.11.1 - table 16-1
	SET(TCCR1A, COM1A1); // set toggle OC1A on compare match bit 1
	RESET(TCCR1A, COM1A0); // set toggle OC1A on compare match bit 2

	// set counter mode operation -> doc : 16.11.1 - table 16-4
	SET(TCCR1B, WGM13); // set bit 3
	SET(TCCR1B, WGM12); // set bit 2
	SET(TCCR1A, WGM11); // set bit 1
	RESET(TCCR1A, WGM10); // reset bit 0

	ICR1 = BLINK_SPEED; // set TOP value to ICR1 (1 Hz)
	OCR1A = 0;
}


ISR(TIMER0_COMPA_vect)
{
	volatile static uint8_t	duty_percent = 0;

	if (duty_percent < 100)
		OCR1A = BLINK_SPEED * ((float)duty_percent / 100); // set duty cycle up
	else
		OCR1A = BLINK_SPEED * ((float)(200 - duty_percent) / 100); // set duty cycle down
	duty_percent++;
	if (duty_percent > 200)
		duty_percent = 0;
}

void	init_interrupt()
{
	SET(SREG, 7);	// activate global interrupt
}

int main()
{
	SET(DDRB, PB1); // set PB1 to output

	init_timer0();
	init_timer1();
	init_interrupt();
	while (1);
}