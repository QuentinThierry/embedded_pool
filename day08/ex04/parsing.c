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

bool	full_rainbow()
{
	if (uart_rtx() == 'L' &&
		uart_rtx() == 'L' &&
		uart_rtx() == 'R' &&
		uart_rtx() == 'A' &&
		uart_rtx() == 'I' &&
		uart_rtx() == 'N' &&
		uart_rtx() == 'B' &&
		uart_rtx() == 'O' &&
		uart_rtx() == 'W')
		return true;
	return false;
}

uint32_t	parsing(bool *error, bool *is_full_rainbow)
{
	char	c = 0;
	bool	fullrainbow = true;
	uint32_t	res = 0;

	c = uart_rtx();
	if (c != '#')
	{
		*error = true;
		return 0;
	}

	//first letter
	c = uart_rtx();
	if (c != 'F')
		fullrainbow = false;
	res = is_not_hex(c, error);
	if (*error == true)
		return 0;
	// second letter
	c = uart_rtx();
	if (c == 'U' && fullrainbow)
	{
		*is_full_rainbow = full_rainbow();
		if (*is_full_rainbow == false)
			*error = true;
		return 0;
	}
	uint8_t i = 0; // letters from 2nd to 6th
	while (!*error && i < 5)
	{
		res <<= 4;
		res |= is_not_hex(c, error);
		if (*error)
			return 0;
		c = uart_rtx();
		i++;
	}
	// two last led letter
	if (c != 'D')
	{
		*error = true;
		return 0;
	}
	c = uart_rtx();
	if (c != '6' && c != '7' && c != '8')
	{
		*error = true;
		return 0;
	}
	res <<= 8;
	res |= c - '0';
	return res;
}
