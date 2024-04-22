#include "main.h"
#include <util/delay.h>

#define ADDR_EEPROM (uint8_t *)568

struct bit_field
{
	uint8_t	val : 4;
};

void	change_display(uint8_t value)
{
	PORTB = (value & 1) | (value & (1 << 1)) // add to the saved bits the bit of the value we get, 1 by 1
			| (value & (1 << 2)) | ((value & (1 << 3)) << 1)
			| (PORTB & ~(0b10111));
}

int	main()
{
	uart_init();

	RESET(DDRD, DDD2);
	SET(DDRB, DDB0);
	SET(DDRB, DDB1);
	SET(DDRB, DDB2);
	SET(DDRB, DDB4);

	struct bit_field value = {0};
	value.val = eeprom_read_byte(ADDR_EEPROM);
	
	change_display(value.val);
	while (1)
	{
		while ((PIND & (1 << PD2)) != 0);
		_delay_ms(50);

		value.val = eeprom_read_byte(ADDR_EEPROM);
		value.val++;
		eeprom_write_byte(ADDR_EEPROM, value.val);
		change_display(value.val);
		while ((PIND & (1 << PD2)) == 0);
		_delay_ms(50);
	}
}
