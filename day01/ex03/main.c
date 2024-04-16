#include "main.h"
#include <util/delay.h>

#define NB_CYCLE_ONE_SECOND 62500

int main()
{
	SET(DDRB, PB1); // set PB1 to output

	// prescaler -> doc : 16.11.2
	SET(TCCR1B, CS12); // set prescaling to F_CPU/256 -> 62500 bit 1
	RESET(TCCR1B, CS11); // set prescaling to F_CPU/256 -> 62500 bit 2
	RESET(TCCR1B, CS10); // set prescaling to F_CPU/256 -> 62500 bit 3

	// compare match -> doc : 16.11.1 - table 16-1
	SET(TCCR1A, COM1A1); // set toggle OC1A on compare match bit 1
	RESET(TCCR1A, COM1A0); // set toggle OC1A on compare match bit 2

	// set counter mode operation -> doc : 16.11.1 - table 16-4
	SET(TCCR1B, WGM13); // set bit 1
	SET(TCCR1B, WGM12); // set bit 1
	SET(TCCR1A, WGM11); // set bit 1
	RESET(TCCR1A, WGM10); // reset bit 1

	ICR1 = NB_CYCLE_ONE_SECOND;  // assign TOP value
	OCR1A = 0; // set the compare time at 0

	uint8_t	pressedSW1 = 0;
	uint8_t	pressedSW2 = 0;
	uint8_t	percent = 0;

	while (1)
	{
		if (pressedSW1 == 0 && (PIND & (1 << PD2)) == 0) // if the SW1 switch button is pressed
		{
			pressedSW1 = 1;
			if (percent < 100)
				percent += 10;
			OCR1A = NB_CYCLE_ONE_SECOND / 100 * percent;  // assign a percentage of the 1 second duty cycle
		}
		else if ((PIND & (1 << PD2)) != 0) // else if not pressed, once
			pressedSW1 = 0;
		if (pressedSW2 == 0 && (PIND & (1 << PD4)) == 0) // if the SW2 switch button is pressed
		{
			pressedSW2 = 1;
			if (percent > 0)
				percent -= 10;
			OCR1A = NB_CYCLE_ONE_SECOND / 100 * percent; // assign a percentage of the 1 second duty cycle
		}
		else if ((PIND & (1 << PD4)) != 0) // else if not pressed, once
			pressedSW2 = 0;
		_delay_ms(10); // adding delay of 10 ms to debounce
	}
}
