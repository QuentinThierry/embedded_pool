#include "main.h"

uint8_t	is_not_hex(uint8_t c, bool *error)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	*error = true;
	return 0;
}

uint32_t	parsing()
{
	uint32_t val = 0;
	uint8_t c = 0;
	bool	error = false;

	c = uart_rtx();
	if (c != '#')
		return UINT32_MAX;
	for (uint8_t i = 0; i < 6; i++) {
		c = uart_rtx();
		c = is_not_hex(c, &error);
		if (error)
			return UINT32_MAX;
		val |= c;
		if (i != 5)
			val <<= 4;
	}
	return val;
}

void	reset_display()
{
	uart_printstr("\r\n> ");
}
