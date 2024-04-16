#include <avr/io.h>

int main()
{
	DDRB	|=	(1 << PB0); // set pin 0 of port B as output
	PORTB	|=	(1 << PB0); // to write data on port B pin 0
	while (1);
}
