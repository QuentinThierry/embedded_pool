#include "main.h"
#include <util/delay.h>

#define MAGIC_NUMBER 0x1a
#define MAGIC_NUMBER_SIZE 8
#define MEMORY_SIZE 1024

void	write_magic_number(uint8_t *addr)
{
	eeprom_update_byte(addr, MAGIC_NUMBER);
}

bool	check_is_magic_number(uint8_t *addr)
{
	if (eeprom_read_byte(addr) == MAGIC_NUMBER)
		return true;
	return  false;
}

bool	safe_eeprom_read(void *buffer, size_t offset, size_t length)
{
	if (offset >= MEMORY_SIZE - length - 1)
		return false;
	if (!check_is_magic_number((void *)offset))
		return false;
	offset += 1;
	eeprom_read_block(buffer, (void *)offset, length);
	return true;
}

bool	safe_eeprom_write(void *buffer, size_t offset, size_t length)
{
	if (offset >= MEMORY_SIZE - length - 1)
		return false;
	write_magic_number((void *)offset);
	offset += 1;
	eeprom_update_block(buffer, (void *)offset, length);
	return true;
}

int	main()
{
	uart_init();
	_delay_ms(100);
	if (!safe_eeprom_write("coucou :)", 15, 10))
		uart_printstr("ERROR WRITE\r\n");

	char buffer[10] = {0};
	if (!safe_eeprom_read(buffer, 15, 10))
		uart_printstr("ERROR READ\r\n");

	uart_printstr(buffer);
	while (1);
}
