#include "main.h"
#include <util/delay.h>

#define MAGIC_NUMBER (0x1a << 1)
#define MEMORY_SIZE 1024
#define HEADER_ALL 5
#define HEADER_MAGIC 1
#define HEADER_ID 2
#define HEADER_LEN 2
#define FREE 1
#define ALLOC 0

void	write_magic_number(uint8_t *addr)
{
	eeprom_update_byte(addr, MAGIC_NUMBER);
}

bool	check_is_magic_number(uint8_t *addr)
{
	if ((eeprom_read_byte(addr) & 0xfe) == MAGIC_NUMBER)
		return true;
	return  false;
}

bool	check_magic_is_free(void *addr)
{
	if ((eeprom_read_byte(addr) & 1) == FREE)
		return true;
	return false;
}

bool	has_space_to_write(void *magic_addr, uint16_t length_to_write, uint16_t previous_len)
{
	uint16_t *len_addr = magic_addr;
	bool	res = true;

	if ((previous_len < length_to_write || previous_len - length_to_write < HEADER_ALL + 1) && length_to_write != previous_len)
	{
		return false;
	}
	if (length_to_write == previous_len)
	{
		if ((size_t)magic_addr + HEADER_ALL + length_to_write + HEADER_ALL + 1 >= MEMORY_SIZE)
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
	addr += HEADER_MAGIC + HEADER_ID;
	return eeprom_read_word((void *)addr);
}

uint16_t	get_chunk_id(uint8_t *addr)
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

void	write_header(uint8_t *addr, uint16_t id, uint16_t len, bool is_free)
{
	eeprom_update_word((void *)addr, MAGIC_NUMBER | is_free);
	addr += HEADER_MAGIC;
	eeprom_update_word((void *)addr, id);
	addr += HEADER_ID;
	eeprom_update_word((void *)addr, len);
}

bool eepromalloc_write(uint16_t id, void *buffer, uint16_t length)
{
	// find a free space -> find magic number + free
	uint8_t		*addr = 0;
	uint16_t	previous_len = 0;

	while ((size_t)addr < MEMORY_SIZE - length - 5)
	{
		if (!check_is_magic_number(addr)) // if not a magic number
		{
			write_header(addr, id, length, false); // write new header
			if (safe_eeprom_write(buffer, addr + HEADER_ALL, length)) // rewrite buffer
				break ;
			else
				return false;
		}
		else if ((previous_len = get_chunk_len(addr)) // find previous size
				&& check_magic_is_free(addr) // check if its a free'd block
				&& has_space_to_write(addr, length, previous_len)) // check if enough space to write block
		{
			write_header(addr, id, length, false); // rewrite previous header
			addr += HEADER_ALL;
			eeprom_update_block(buffer, addr, length); // rewrite buffer
			addr += length;
			if (!check_is_magic_number(addr)) // if it's not a magic number, so the previous memory had not the very same size
			{
				write_header(addr, 0, previous_len - length - HEADER_ALL, true); // append new header after new buffer
			}
			break ;
		}
		addr += HEADER_ALL + previous_len; // skip to next block
	}
	if ((size_t)addr >= MEMORY_SIZE - length - 5)
		return false;
	return true;
}

bool eepromalloc_read(uint16_t id, void *buffer, uint16_t length)
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
			addr += HEADER_ALL + get_chunk_len(addr);
			continue ;
		}
		else
		{
			if (get_chunk_id(addr) == id)
			{
				safe_eeprom_read(buffer, addr + HEADER_ALL, length);
				return true;
			}
			addr += HEADER_ALL + get_chunk_len(addr);
		}
	}
	if ((size_t)addr >= MEMORY_SIZE - length - 5)
		return false;
	return true;
}

bool eepromalloc_free(uint16_t id)
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
			addr += HEADER_LEN + get_chunk_len(addr);
			continue ;
		}
		else
		{
			if (get_chunk_id(addr) == id)
			{
				write_header(addr, id, get_chunk_len(addr), true);
				return true;
			}
			addr += HEADER_ALL + get_chunk_len(addr);
		}
	}
	return true;
}

int	main()
{
	uart_init();


	char empty[100] = {0};
	safe_eeprom_write(empty, 0, 100);

	uart_printstr("cleared\r\n");

	{
		char buffer[1024] = {0};
		eepromalloc_write(42, buffer, 1024);
		eepromalloc_free(42);
	}

	if (!eepromalloc_write(0, "premiere chaine de caractere\n\r", 30))
		uart_printstr("ERROR WRITE\r\n");

	if (!eepromalloc_write(1, "deuxieme\r\n", 11))
		uart_printstr("ERROR WRITE\r\n");

	{
		char buffer[15] = {0};
		uart_printstr("Reading id 1 : \r\n\t");
		if (!eepromalloc_read(1, buffer, 11))
			uart_printstr("ERROR READ\r\n");
		for (int i = 0; i < 11; i++) {
			uart_tx(buffer[i]);
		}
	}

	{
		char buffer[30] = {0};
		uart_printstr("Reading id 0 : \r\n\t");
		if (!eepromalloc_read(0, buffer, 30))
			uart_printstr("ERROR READ\r\n");
		for (int i = 0; i < 30; i++) {
			uart_tx(buffer[i]);
		}
	}

	uart_printstr("Freeing id 0 : \r\n");
	eepromalloc_free(0);
	{
		char buffer[15] = {0};
		uart_printstr("Reading id 0 (free'd) : \r\n");
		if (!eepromalloc_read(0, buffer, 7))
			uart_printstr("ERROR READ\r\n");
		for (int i = 0; i < 15; i++) {
			uart_tx(buffer[i]);
		}
	}

	if (!eepromalloc_write(0, "touta\r\n", 7))
		uart_printstr("ERROR WRITE\r\n");
	if (!eepromalloc_write(2, "salut\r\n", 7))
		uart_printstr("ERROR WRITE\r\n");

	{
		char buffer[15] = {0};
		uart_printstr("Reading id 0 (realloc'd): \r\n\t");
		if (!eepromalloc_read(0, buffer, 7))
			uart_printstr("ERROR READ\r\n");
		for (int i = 0; i < 15; i++) {
			uart_tx(buffer[i]);
		}
	}

	{
		char buffer[15] = {0};
		uart_printstr("Reading id 1 : \r\n\t");
		if (!eepromalloc_read(1, buffer, 11))
			uart_printstr("ERROR READ\r\n");
		for (int i = 0; i < 15; i++) {
			uart_tx(buffer[i]);
		}
	}


	{
		char buffer[15] = {0};
		uart_printstr("Reading id 2 (realloc'd): \r\n\t");
		if (!eepromalloc_read(2, buffer, 7))
			uart_printstr("ERROR READ\r\n");
		for (int i = 0; i < 15; i++) {
			uart_tx(buffer[i]);
		}
	}

	while (1);
}
