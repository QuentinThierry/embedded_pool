#include "main.h"
#define CLOCK_ONE_SEC (F_CPU / 24)

int main()
{
	unsigned long compter = 0;
	DDRB |= (1 << PB1);
	SET(DDRB, PB1);

	while (1)
	{
		if (compter == CLOCK_ONE_SEC)
		{
			TOGGLE(PORTB, PB1);
			compter = 0;
		}
		compter++;
	}
}
