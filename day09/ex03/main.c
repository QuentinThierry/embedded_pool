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

#define ZERO	(1) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5)
#define ONE		(1 << 1) | (1 << 2)
#define TWO		(1) | (1 << 1) | (1 << 6) | (1 << 4) | (1 << 3)
#define THREE	(1) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 6)
#define FOUR	(1 << 1) | (1 << 2) | (1 << 5) | (1 << 6)
#define FIVE	(1) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6)
#define SIX		(1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6)
#define SEVEN	(1) | (1 << 1) | (1 << 2)
#define EIGHT	(1) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6)
#define NINE	(1) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6)

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

void	display_num(uint8_t num)
{
	send_command_write(CONFIG_CMD_P0, (1 << DIGIT3));
	send_command_write(OUT_REGISTER_P0, (1 << DIGIT3));

	uint8_t digit = 0;

	switch (num) {
		case 0:
			digit = ZERO;
			break;
		case 1:
			digit = ONE;
			break;
		case 2:
			digit = TWO;
			break;
		case 3:
			digit = THREE;
			break;
		case 4:
			digit = FOUR;
			break;
		case 5:
			digit = FIVE;
			break;
		case 6:
			digit = SIX;
			break;
		case 7:
			digit = SEVEN;
			break;
		case 8:
			digit = EIGHT;
			break;
		case 9:
			digit = NINE;
			break;
	}
	send_command_write(CONFIG_CMD_P1, digit | 0x80);
	send_command_digit(OUT_REGISTER_P1, digit);
}

int	main()
{
	i2c_init();
	_delay_ms(50);

	uint8_t i = 0;
	while (1)
	{
		display_num(i);
		i++;
		if (i == 10)
			i = 0;
		_delay_ms(1000);
	}
}
