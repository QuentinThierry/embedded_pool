#include "main.h"
#include <util/delay.h>

#define ADDR_COUNTERS (uint8_t *)0x0
#define ADDR_COUNTER0 (uint8_t *)0x1
#define ADDR_COUNTER1 (uint8_t *)0x2
#define ADDR_COUNTER2 (uint8_t *)0x3
#define ADDR_COUNTER3 (uint8_t *)0x4

struct bit_field4
{
	uint8_t	val : 4;
};

struct bit_field2
{
	uint8_t	val : 2;
};

void	change_display(uint8_t value)
{
	PORTB = (value & 1) | (value & (1 << 1)) // add to the saved bits the bit of the value we get, 1 by 1
			| (value & (1 << 2)) | ((value & (1 << 3)) << 1)
			| (PORTB & ~(0b10111));
}

void	display_counter_val()
{
	struct bit_field2 counter_id = {eeprom_read_byte(ADDR_COUNTERS)};

	switch(counter_id.val)
	{
		case 0:
			change_display(eeprom_read_byte(ADDR_COUNTER0));
			break;
		case 1:
			change_display(eeprom_read_byte(ADDR_COUNTER1));
			break;
		case 2:
			change_display(eeprom_read_byte(ADDR_COUNTER2));
			break;
		case 3:
			change_display(eeprom_read_byte(ADDR_COUNTER3));
			break;
		default:
			break;
	}
}

uint8_t	get_counter_val()
{
	return eeprom_read_byte(ADDR_COUNTERS);
}

void	switch_counter()
{
	struct bit_field2 counter_id = {eeprom_read_byte(ADDR_COUNTERS)};

	counter_id.val++;
	eeprom_write_byte(ADDR_COUNTERS, counter_id.val);
	display_counter_val();
}

void	add_to_current_counter()
{
	struct bit_field4 counter_val = {0};

	switch (get_counter_val())
	{
		case 0:
			counter_val.val = eeprom_read_byte(ADDR_COUNTER0);
			eeprom_write_byte(ADDR_COUNTER0, counter_val.val + 1);
			break;
		case 1:
			counter_val.val = eeprom_read_byte(ADDR_COUNTER1);
			eeprom_write_byte(ADDR_COUNTER1, counter_val.val + 1);
			break;
		case 2:
			counter_val.val = eeprom_read_byte(ADDR_COUNTER2);
			eeprom_write_byte(ADDR_COUNTER2, counter_val.val + 1);
			break;
		case 3:
			counter_val.val = eeprom_read_byte(ADDR_COUNTER3);
			eeprom_write_byte(ADDR_COUNTER3, counter_val.val + 1);
			break;
		default:
			break;
	}
	change_display(counter_val.val + 1);
}

int	main()
{
	RESET(DDRD, DDD2);
	RESET(DDRD, DDD4);
	SET(DDRB, DDB0);
	SET(DDRB, DDB1);
	SET(DDRB, DDB2);
	SET(DDRB, DDB4);

	display_counter_val();
	while (1)
	{
		if ((PIND & (1 << PD2)) == 0)
		{
			add_to_current_counter();
			_delay_ms(200);
			while ((PIND & (1 << PD2)) == 0);
		}
		else if ((PIND & (1 << PD4)) == 0)
		{
			switch_counter();
			_delay_ms(200);
			while ((PIND & (1 << PD4)) == 0);
		}
		
	}

}
