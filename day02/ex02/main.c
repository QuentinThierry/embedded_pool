#include "main.h"

struct bit_field
{
	uint8_t	val : 4;
};

volatile struct bit_field value = {0};

void	change_display(struct bit_field value)
{
	PORTB = (value.val & 1) | (value.val & (1 << 1)) // add to the saved bits the bit of the value we get, 1 by 1
			| (value.val & (1 << 2)) | ((value.val & (1 << 3)) << 1)
			| (PORTB & ~(0b10111));
}

ISR(INT0_vect)
{
	value.val++;
	change_display(value);
}

ISR(PCINT2_vect)
{
	static uint8_t is_falling = 0;
	is_falling = ~is_falling;
	if (is_falling == 0)
		return ;
	value.val--;
	change_display(value);
}

void	init_interruptSW1()
{
	SET(EICRA, ISC01);		// set interrupt as falling edge -> doc : 13.2.1
	RESET(EICRA, ISC00);	// set interrupt as falling edge -> doc : 13.2.1
	SET(EIMSK, INT0); // set external pin interrupt enable
}

void	init_interruptSW2()
{
	SET(PCICR, PCIE2); // set bit mask to INT2 -> doc : 13.2.4
	SET(PCMSK2, PCINT20); // set bit mask to PCINT20 -> doc : 13.2.6
}

int main()
{
	SET(DDRB, PB0); // set PB0 to output
	SET(DDRB, PB1); // set PB1 to output
	SET(DDRB, PB2); // set PB2 to output
	SET(DDRB, PB4); // set PB3 to output
	SET(SREG, 7); // activate global interrupt

	init_interruptSW1();
	init_interruptSW2();


	while (1);
}