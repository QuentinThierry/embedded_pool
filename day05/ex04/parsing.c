#include "main.h"

bool	parse_write(char *str)
{
	bool is_ok = true;
	uint16_t i = 0;
	char c = 0;

	if (uart_rtx() != 'R')
		return false;
	if (uart_rtx() != 'I')
		return false;
	if (uart_rtx() != 'T')
		return false;
	if (uart_rtx() != 'E')
		return false;
	c = uart_rtx();
	while (c != ' ')
	{
		is_ok = false;
		c = uart_rtx();
	}
	while (c == ' ')
		c = uart_rtx();
	while (c != '\"')
	{
		is_ok = false;
		c = uart_rtx();
	}
	while (1) // delim
	{
		
		c = uart_rtx();
		if (c == '\"')
			break ;
		if (i >= 100)
			return false;
		str[i] = c;
		i++;
	}
	str[i] = 0;
	if (i == 0)
	{
		uart_printstr("\r\nEmpty field\r\n");
		return false;
	}
	
	i++;
	c = uart_rtx();
	while (c == ' ')
		c = uart_rtx();
	
	while (1) // memory
	{
		c = uart_rtx();
		if (c == '\"')
			break ;
		if (i >= 100)
			return false;
		str[i] = c;
		i++;
	}
	if (i == 1)
	{
		uart_printstr("\r\nEmpty field\r\n");
		return false;
	}
	return is_ok;
}

void	exec_write(char *str)
{
	char buffer[100] = {0};
	char buffer2[100] = {0};
	uint16_t i = 0;
	uint16_t j = 0;

	while (i < 100 && str[i] != 0)
	{
		buffer[i] = str[i];
		i++;
	}
	buffer[i] = 0;
	j = 0;
	while (j + i + 1 < 100 && str[j + i + 1] != 0)
	{
		buffer2[j] = str[j + i + 1];
		j++;
	}
	buffer2[j] = 0;
	if (!eepromalloc_write(buffer, buffer2, j))
	{
		uart_printstr("\r\nAlready used id or no space left.");
		return ;
	}
	uart_printstr("\r\nDone.");
}

bool	parse_read(char *str)
{
	bool is_ok = true;
	uint16_t i = 0;
	char c = 0;

	if (uart_rtx() != 'E')
		return false;
	if (uart_rtx() != 'A')
		return false;
	if (uart_rtx() != 'D')
		return false;
	c = uart_rtx();
	while (c != ' ')
	{
		is_ok = false;
		c = uart_rtx();
	}
	while (c == ' ')
		c = uart_rtx();
	while (c != '\"')
	{
		is_ok = false;
		c = uart_rtx();
	}
	while (c == ' ')
		c = uart_rtx();
	while (1)
	{
		c = uart_rtx();
		if (c == '\"')
			break ;
		if (i >= 100)
			return false;
		str[i] = c;
		i++;
	}
	str[i] = 0;
	if (i == 0)
		return false;
	return is_ok;
}

void	exec_read(char *str)
{
	char buffer[100] = {0};
	if (!eepromalloc_read(str, buffer, 100))
	{
		uart_printstr("\r\n");
		return ;
	}
	uart_printstr("\r\n\"");
	uart_printstr(buffer);
	uart_printstr("\"");
}

bool	parse_forget(char *str)
{
	bool is_ok = true;
	uint16_t i = 0;
	char c = 0;

	if (uart_rtx() != 'O')
		return false;
	if (uart_rtx() != 'R')
		return false;
	if (uart_rtx() != 'G')
		return false;
	if (uart_rtx() != 'E')
		return false;
	if (uart_rtx() != 'T')
		return false;
	c = uart_rtx();
	while (c != ' ')
	{
		is_ok = false;
		c = uart_rtx();
	}
	while (c == ' ')
		c = uart_rtx();
	while (c != '\"')
	{
		is_ok = false;
		c = uart_rtx();
	}
	while (1)
	{
		c = uart_rtx();
		if (c == '\"')
			break ;
		if (i >= 100)
			return false;
		str[i] = c;
		i++;
	}
	if (i == 0)
		return false;
	return is_ok;
}

void	exec_forget(char *str)
{
	if (eepromalloc_free(str))
		uart_printstr("\r\nDone.");
	else
		uart_printstr("\r\nNot found.");
}


void	parse_command()
{
	char c = uart_rtx();
	char str[200] = {0};

	if (c != '\"')
	{
		if (c == 'R')
		{
			if (parse_read(str))
				exec_read(str);
		}
		else if (c == 'W')
		{
			if (parse_write(str))
				exec_write(str);
		}
		else if (c == 'F')
		{
			if (parse_forget(str))
				exec_forget(str);
		}
		else
		{
			while (c != '\"')
				c = uart_rtx();
		}
	}
}

void	reset_display()
{
	uart_printstr("\r\n> ");
}
