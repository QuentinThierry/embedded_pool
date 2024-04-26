#include "main.h"

#define BASE_ADDR (0b0100 << 3)
// #define A0 0b001
#define A0 0b000
// #define A1 0b010
#define A1 0b000
// #define A2 0b100
#define A2 0b000

#define OUT_REGISTER_P0 2
#define OUT_REGISTER_P1 3

#define CONFIG_CMD_P0 6
#define CONFIG_CMD_P1 7


void	reset_pins(uint8_t command)
{
	i2c_start_write(BASE_ADDR | A0 | A1 | A2);
	i2c_write(command); // command
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

int	main()
{
	i2c_init();

	_delay_ms(50);
	send_command_write(CONFIG_CMD_P0, 0b1000);
	while (1)
	{
		reset_pins(OUT_REGISTER_P0);
		_delay_ms(500);
		send_command_write(OUT_REGISTER_P0, 0b1000);
		_delay_ms(500);
	}
}
