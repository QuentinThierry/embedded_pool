#include <avr/io.h>
#include <util/delay.h>

int main()
{
	short	pressed = 0;

	DDRB	=	(1 << PB0); // set pin PB0 of port B as output
	DDRD	=	(0 << PD2); // set pin PD2 of port D as input
	while (1)
	{
		if (pressed == 0 && (PIND & (1 << PD2)) == 0) // if the PD2 switch button is pressed
		{
			pressed = 1;
			PORTB ^= (1 << PB0); // invert the value of the led PB0
		}
		else if (pressed == 1 && (PIND & (1 << PD2)) != 0) // else if PD2 is not pressed
		{
			pressed = 0;
		}
		_delay_ms(10); // adding delay of 10 ms to prevent overheat and bounce
	}
}
