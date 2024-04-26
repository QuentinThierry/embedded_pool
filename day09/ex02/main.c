#include "main.h"

#define BASE_ADDR (0b0100 << 3)
#define A0 0b000
#define A1 0b000
#define A2 0b000
#define SLA_ADDR (BASE_ADDR | A0 | A1 | A2)

#define DEACTIVATE 0
#define ACTIVATE 1

#define IN_REGISTER_P0 0
#define IN_REGISTER_P1 1
#define OUT_REGISTER_P0 2
#define OUT_REGISTER_P1 3

#define CONFIG_CMD_P0 6
#define CONFIG_CMD_P1 7

#define DIGIT0 4
#define DIGIT1 5
#define DIGIT2 6
#define DIGIT3 7


void	reset_pins(uint8_t port)
{
	i2c_start_write(SLA_ADDR);
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
	i2c_start_write(SLA_ADDR);
	i2c_write(cmd); // command
	i2c_write(0xff & ~pins); // set pins
	i2c_stop();
	_delay_ms(50);
}

void	send_command_digit(uint8_t cmd, uint8_t pins)
{
	i2c_start_write(SLA_ADDR);
	i2c_write(cmd); // command
	i2c_write(pins); // set pins
	i2c_stop();
	_delay_ms(50);
}

uint8_t	read_value()
{
	i2c_start_write(SLA_ADDR);
	i2c_write(IN_REGISTER_P0); // command
	i2c_start_read(SLA_ADDR);
	uint8_t res = i2c_read(false);
	i2c_stop();
	_delay_ms(50);
	return res;
}

void	display_two()
{
	send_command_write(CONFIG_CMD_P0, (1 << DIGIT3));
	send_command_write(OUT_REGISTER_P0, (1 << DIGIT3));
	send_command_write(CONFIG_CMD_P1, 0xff);
	send_command_digit(OUT_REGISTER_P1, 1 | (1 << 1) | (1 << 6) | (1 << 4) | (1 << 3));
}

int	main()
{
	i2c_init();
	_delay_ms(50);

	display_two();
	while (1);
}
