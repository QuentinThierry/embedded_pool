#include "main.h"
#include <util/delay.h>

int main()
{

	SET(DDRB, PB1); // set PB1 to output

	// prescaler -> doc : 16.11.2
	SET(TCCR1B, CS12); // set prescaling to F_CPU/256 -> 62500 bit 1
	RESET(TCCR1B, CS11); // set prescaling to F_CPU/256 -> 62500 bit 2
	RESET(TCCR1B, CS10); // set prescaling to F_CPU/256 -> 62500 bit 3

	// compare match -> doc : 16.11.1 - table 16-1
	RESET(TCCR1A, COM1A1); // set toggle OC1A on compare match bit 1
	SET(TCCR1A, COM1A0); // // set toggle OC1A on compare match bit 2

	// set counter mode operation -> doc : 16.11.1 - table 16-4
	RESET(TCCR1B, WGM13); // reset bit 1
	SET(TCCR1B, WGM12); // set bit 2
	RESET(TCCR1A, WGM11); // reset bit 3
	RESET(TCCR1A, WGM10); // reset bit 4

	OCR1A = 31250;

	while (1);
}
