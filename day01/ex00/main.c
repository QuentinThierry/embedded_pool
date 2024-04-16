#include "main.h"

int main()
{
	unsigned long compter = 0;
	DDRB |= (1 << PB1);
	SET(DDRB, PB1);

	while (1)
	{
		if (compter >= 750000ul)
		{
			TOGGLE(PORTB, PB1);
			compter = 0;
		}
		compter++;
	}
}
