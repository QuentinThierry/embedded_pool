#include "main.h"

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

		rgb_reset_all();

		// yellow
		set_red();
		set_green();
		_delay_ms(1000);
		rgb_reset_all();
		// cyan
		set_green();
		set_blue();
		_delay_ms(1000);
		rgb_reset_all();
		// magenta
		set_red();
		set_blue();
		_delay_ms(1000);
		rgb_reset_all();
		// white
		set_blue();
		set_green();
		set_red();
		_delay_ms(1000);
		rgb_reset_all();
	}
}
