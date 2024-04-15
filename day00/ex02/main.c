#include <avr/io.h>
#include <util/delay.h>

int main()
{
	DDRB	=	(1 << PB0); // set pin PB0 of port B as output
	DDRD	=	(0 << PD2); // set pin PD2 of port D as input
	while (1)
	{
		if ((PIND & (1 << PD2)) == 0) // if the PD2 switch button is pressed
		{
			PORTB	|=	(1 << PB0); // to write data on pin PB0 port B 
		}
		else if (PORTB & (1 << PB0) == 1) // if the PD2 is not pressed and led D1 is on
		{
			PORTB	&=	~(1 << PB0); // to remove data write on PB0
		}
		_delay_ms(10); // adding delay of 10 ms to prevent overheat
	}
}
