#include "main.h"

#define BASE_ADDR (0b0100 << 3)
// #define A0 0b001
#define A0 0b000
// #define A1 0b010
#define A1 0b000
// #define A2 0b100
#define A2 0b000

#define DEACTIVATE 0
#define ACTIVATE 1

#define IN_REGISTER_P0 0
#define IN_REGISTER_P1 1
#define OUT_REGISTER_P0 2
#define OUT_REGISTER_P1 3

#define CONFIG_CMD_P0 6
#define CONFIG_CMD_P1 7

struct bit_field
{
	uint8_t val : 3;
};

void	reset_pins(uint8_t port)
{
	i2c_start_write(BASE_ADDR | A0 | A1 | A2);
	if (port == 0)
		i2c_write(OUT_REGISTER_P0); // command
	else
		i2c_write(OUT_REGISTER_P1); // command
	i2c_write(0xff); // set pins
	i2c_stop();
	_delay_ms(50);
}

void	send_command_write(uint8_t cmd, uint8_t pins)
{
	i2c_start_write(BASE_ADDR | A0 | A1 | A2);
	i2c_write(cmd); // command
	i2c_write(0xff & ~pins); // set pins
	i2c_stop();
	_delay_ms(50);
}

uint8_t	read_value()
{
	i2c_start_write(BASE_ADDR | A0 | A1 | A2);
	i2c_write(IN_REGISTER_P0); // command
	i2c_start_read(BASE_ADDR | A0 | A1 | A2);
	uint8_t res = i2c_read(false);
	i2c_stop();
	_delay_ms(50);
	return res;
}

void	display_value(uint8_t val)
{
	uint8_t pins = 0;
	if ((val & 1) == 1)
		pins |= 0b0010;
	if ((val & 2) == 2)
		pins |= 0b0100;
	if ((val & 4) == 4)
		pins |= 0b1000;
	send_command_write(OUT_REGISTER_P0, pins);
}

int	main()
{
	i2c_init();
	struct bit_field value = {0};

	_delay_ms(50);
	reset_pins(0);
	reset_pins(1);
	send_command_write(CONFIG_CMD_P0, 0b1110);
	while (1)
	{
		if ((read_value() & 1) == 0)
		{
			value.val++;
			display_value(value.val);
			while ((read_value() & 1) == 0);
		}
		else
			display_value(value.val);
		_delay_ms(20);
	}
	
}
