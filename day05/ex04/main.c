#include "main.h"
#include <util/delay.h>

#define MAGIC_NUMBER (0x1a << 1)
#define MEMORY_SIZE 1024
#define HEADER_ALL 3
#define HEADER_MAGIC 1
#define HEADER_LEN 2
#define FREE 1
#define ALLOC 0

uint8_t	ft_strlen(char *str)
{
	uint8_t	i = 0;
	while (str[i])
		i++;
	return i;
}

void	write_magic_number(uint8_t *addr)
{
	eeprom_update_byte(addr, MAGIC_NUMBER);
}

bool	check_is_magic_number(uint8_t *addr)
{
	if ((eeprom_read_byte(addr) & 0xfe) == MAGIC_NUMBER)
		return true;
	return false;
}

bool	check_magic_is_free(void *addr)
{
	if ((eeprom_read_byte(addr) & 1) == FREE)
		return true;
	return false;
}

bool	has_space_to_write(void *magic_addr, uint16_t length_to_write, uint16_t previous_len)
{
	if (length_to_write != previous_len)
	{
		return false;
	}
	else
	{
		if ((size_t)magic_addr + HEADER_ALL + length_to_write >= MEMORY_SIZE)
			return false;
	}
	return true;
}

uint16_t	get_chunk_len(uint8_t *addr)
{
	addr += HEADER_MAGIC;
	return eeprom_read_word((void *)addr);
}

bool	safe_eeprom_read(void *buffer, uint8_t *addr, size_t length)
{
	if ((size_t)addr >= MEMORY_SIZE - length - 1)
		return false;
	eeprom_read_block(buffer, (void *)addr, length);
	return true;
}

bool	safe_eeprom_write(void *buffer, uint8_t *addr, size_t length)
{
	if ((size_t)addr + length >= MEMORY_SIZE)
		return false;
	eeprom_update_block(buffer, addr, length);
	return true;
}

bool	compare_id(uint8_t *magic_addr, char *buffer)
{
	magic_addr += HEADER_MAGIC + HEADER_LEN;
	char c;
	while (1)
	{
		if (!safe_eeprom_read(&c, (void *)magic_addr, 1))
			return false;
		if (c != *buffer)
		{
			return false;
		}
		if (c == '\0')
		{
			return true;
		}
		magic_addr++;
		buffer++;
	}
}

uint8_t	get_chunk_delim_size(uint8_t *magic_addr)
{
	magic_addr += HEADER_ALL;
	uint8_t i = 0;

	char c;
	while (1)
	{
		safe_eeprom_read(&c, magic_addr + i, 1);
		if (c == 0)
			return i;
		i++;
	}
}

bool	check_id_already_used(char *id)
{
	uint8_t *addr = 0;

	while ((size_t)addr < MEMORY_SIZE)
	{
		if (!check_is_magic_number(addr))
		{
			return false;
		}
		if (check_magic_is_free(addr))
		{
			addr += HEADER_ALL + get_chunk_delim_size(addr) + 1 + get_chunk_len(addr);
			continue;
		}
		if (compare_id(addr, id))
		{
			return true;
		}
		else
		{
			addr += HEADER_ALL + get_chunk_delim_size(addr) + 1 + get_chunk_len(addr);
		}
	}
	return false;
}

void	write_header(uint8_t *addr, char *delim, uint16_t len, bool is_free)
{
	eeprom_update_byte((void *)addr, MAGIC_NUMBER | is_free);
	addr += HEADER_MAGIC;
	eeprom_update_word((void *)addr, len);
	addr += HEADER_LEN;
	safe_eeprom_write(delim, addr, ft_strlen(delim) + 1);
}

bool eepromalloc_write(char *delim, void *buffer, uint16_t length)
{
	// find a free space -> find magic number + free
	uint8_t		*addr = 0;
	uint16_t	previous_len = 0;

	if (check_id_already_used(delim))
		return false;
	while ((size_t)addr < MEMORY_SIZE - length)
	{
		if (!check_is_magic_number(addr)) // if not a magic number
		{
			write_header(addr, delim, length, false); // write new header
			if (safe_eeprom_write(buffer, addr + HEADER_ALL + ft_strlen(delim) + 1, length)) // rewrite buffer
				break ;
			else
				return false;
		}
		else if ((previous_len = get_chunk_len(addr)) // find previous size
				&& check_magic_is_free(addr) // check if its a free'd block
				&& has_space_to_write(addr, length + ft_strlen(delim), previous_len + get_chunk_delim_size(addr))) // check if enough space to write block
		{
			write_header(addr, delim, length, false); // rewrite previous header
			addr += HEADER_ALL + ft_strlen(delim) + 1;
			eeprom_update_block(buffer, addr, length); // rewrite buffer
			break ;
		}
		addr += HEADER_ALL + get_chunk_delim_size(addr) + 1 + previous_len; // skip to next block
	}
	if ((size_t)addr >= MEMORY_SIZE - length)
		return false;
	return true;
}

bool eepromalloc_read(char *delim, void *buffer, uint16_t length)
{
	uint8_t		*addr = 0;

	while ((size_t)addr < MEMORY_SIZE)
	{
		if (!check_is_magic_number(addr))
			return false;
		if (check_magic_is_free(addr))
		{
			addr += HEADER_ALL + get_chunk_delim_size(addr) + 1 + get_chunk_len(addr);
			continue ;
		}
		else
		{
			if (compare_id(addr, delim))
			{
				safe_eeprom_read(buffer, addr + HEADER_ALL + ft_strlen(delim) + 1, length);
				return true;
			}
			addr += HEADER_ALL + get_chunk_delim_size(addr) + 1 + get_chunk_len(addr);
		}
	}
	if ((size_t)addr >= MEMORY_SIZE)
		return false;
	return true;
}

bool eepromalloc_free(char *delim)
{
	uint8_t		*addr = 0;

	while ((size_t)addr < MEMORY_SIZE)
	{
		if (!check_is_magic_number(addr))
		{
			return false;
		}
		if (check_magic_is_free(addr))
		{
			addr += HEADER_ALL + get_chunk_delim_size(addr) + 1 + get_chunk_len(addr);
			continue ;
		}
		else
		{
			if (compare_id(addr, delim))
			{
				write_header(addr, delim, get_chunk_len(addr), true);
				return true;
			}
			addr += HEADER_ALL + get_chunk_delim_size(addr) + 1 + get_chunk_len(addr);
		}
	}
	return true;
}



int	main()
{
	uart_init();

	uart_printstr("> ");

	while (1)
	{
		parse_command();
		reset_display();
	}
}
