#include "main.h"

void	toggle_red()
{
	TOGGLE(PORTD, PD5);
}

void	toggle_blue()
{
	TOGGLE(PORTD, PD3);
}

void	toggle_green()
{
	TOGGLE(PORTD, PD6);
}

int	main()
{
	SET(DDRD, DDD3);
	SET(DDRD, DDD5);
	SET(DDRD, DDD6);

	while (1)
	{
		toggle_red();
		_delay_ms(1000);
		toggle_red();
		toggle_green();
		_delay_ms(1000);
		toggle_green();
		toggle_blue();
		_delay_ms(1000);
		toggle_blue();
	}
}
