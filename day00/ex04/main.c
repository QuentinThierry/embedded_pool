#include <avr/io.h>
#include <util/delay.h>

struct bit_field
{
	unsigned short val : 4;
};

void	change_display(struct bit_field value)
{
	unsigned short res = PORTB & ~(0b10111); //save PORTB bits we don't want to update

	res |= (value.val & 1) | (value.val & (1 << 1)) // add to the saved bits the bit of the value we get, 1 by 1
			| (value.val & (1 << 2)) | ((value.val & (1 << 3)) << 1);
	PORTB = res; // assign res to PORTB
}

int main()
{
	struct bit_field value = {0};
	short pressedSW1 = 0;
	short pressedSW2 = 0;

	DDRB	=	(1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4); // set pin PB0,1,2,4 of port B as output
	DDRD	=	(0 << PD2) | (0 << PD4); // set pin PD2,4 of port D as input
	while (1)
	{
		if (pressedSW1 == 0 && (PIND & (1 << PD2)) == 0) // if the SW1 switch button is pressed
		{
			pressedSW1 = 1;
			value.val++;
			change_display(value);
		}
		else if ((PIND & (1 << PD2)) != 0) // else if not pressed, once
			pressedSW1 = 0;
		if (pressedSW2 == 0 && (PIND & (1 << PD4)) == 0) // if the SW2 switch button is pressed
		{
			pressedSW2 = 1;
			value.val--;
			change_display(value);
		}
		else if ((PIND & (1 << PD4)) != 0) // else if not pressed, once
			pressedSW2 = 0;
		_delay_ms(10); // adding delay of 10 ms to prevent overheat and bounce
	}
}
