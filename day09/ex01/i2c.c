#include "main.h"

void	i2c_init(void)
{
	RESET(TWSR, TWPS0);// set prescaler to 4
	RESET(TWSR, TWPS1);
	SET(TWCR, TWEN);
	TWBR = 72;
}

void	i2c_start_write(uint8_t addr)
{
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1<<TWINT)))
		;
	if (TW_STATUS != TW_START && TW_STATUS != TW_REP_START)
	{
		uart_printstr("ERROR start write : ");
		uart_printhex(TW_STATUS);
		uart_printstr("\r\n");
	};
	
	// set SLA+W mode
	TWDR = (addr << 1); // set slave address
	RESET(TWCR, TWSTA);
	SET(TWCR, TWINT); // clear flag
	while (!(TWCR & (1<<TWINT)))
		;
	if (TW_STATUS != TW_MT_SLA_ACK)
	{
		uart_printstr("ERROR addr write : ");
		uart_printhex(TW_STATUS);
		uart_printstr("\r\n");
	}
}

void	i2c_start_read(uint8_t addr)
{
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1<<TWINT)))
		;
	if (TW_STATUS != TW_START && TW_STATUS != TW_REP_START)
	{
		uart_printstr("ERROR start read : ");
		uart_printhex(TW_STATUS);
		uart_printstr("\r\n");
	}
	
	// set SLA+R mode
	TWDR = (addr << 1) + 1; // set slave address
	RESET(TWCR, TWSTA);
	SET(TWCR, TWINT); // clear flag
	while (!(TWCR & (1<<TWINT)))
		;
	if (TW_STATUS != TW_MR_SLA_ACK)
	{
		uart_printstr("ERROR addr read : ");
		uart_printhex(TW_STATUS);
		uart_printstr("\r\n");
	}
}

void	i2c_stop()
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void	i2c_write(unsigned char data)
{
	TWDR = data;
	SET(TWCR, TWINT);
	while (!(TWCR & (1<<TWINT)))
		;
	if (TW_STATUS != TW_MT_DATA_ACK)
		uart_printstr("ERROR write\r\n");
}

char	i2c_read(bool ack)
{
	if (ack)
		SET(TWCR, TWEA);
	else
		RESET(TWCR, TWEA);
	SET(TWCR, TWINT);
	while (!(TWCR & (1<<TWINT)))
		;
	if (TW_STATUS != (ack ? TW_MR_DATA_ACK : TW_MR_DATA_NACK))
		uart_printstr("ERROR read\r\n");
	unsigned char c = TWDR;
	return c;
}
