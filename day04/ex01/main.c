#include "main.h"

#define COM_FREQUENCY 18

void	uart_init()
{
	const uint16_t	baud = F_CPU / (UART_BAUDRATE * 16);
	UBRR0H = (baud & 0xff00) >> 8; // set baudrate
	UBRR0L = baud & 0xff;
	SET(UCSR0B, TXEN0); // set USART mode to transmit

	RESET(UCSR0C, UMSEL01); // set asynchronous USART mode
	RESET(UCSR0C, UMSEL00);

	RESET(UCSR0C, UPM00); // disable parity mode for 8N1
	RESET(UCSR0C, UPM01);

	RESET(UCSR0C, USBS0); // set 1 stop bit

	SET(UCSR0C, UCSZ00); // set 8 bit size
	SET(UCSR0C, UCSZ01);
	RESET(UCSR0B, UCSZ02);
}


void	i2c_init(void)
{
	RESET(TWSR, TWPS0);// set prescaler to 4
	RESET(TWSR, TWPS1);
	SET(TWCR, TWEN);
	TWBR = 72;
}

void	uart_tx(char c)
{
	while (!(UCSR0A & (1 << UDRE0))); // wait data to be transmit
	UDR0 = c;
}

void	uart_printstr(const char *str)
{
	uint16_t	i = 0;
	while (str[i])
	{
		uart_tx(str[i]);
		i++;
	}
}

void	i2c_printhex(uint8_t data)
{
	const char base[] = "0123456789ABCDEF";
	// uart_printstr("0x");
	uart_tx(base[(data & 0xf0) >> 4]);
	uart_tx(base[(data & 0xf)]);
}

void	i2c_start_write()
{
	SET(TWCR, TWSTA);
	SET(TWCR, TWINT);
	while (!(TWCR & (1<<TWINT)))
		;
	if (TW_STATUS != TW_START && TW_STATUS != TW_REP_START)
	{
		uart_printstr("ERROR! : ");
		i2c_printhex(TW_STATUS);
		uart_printstr("\r\n");
	};
	
	// set SLA+W mode
	TWDR = (0x38 << 1); // set ATH20 slave address
	SET(TWCR, TWINT); // clear flag

	while (!(TWCR & (1<<TWINT)))
		;
	if (TW_STATUS != TW_MT_SLA_ACK)
	{
		uart_printstr("ERROR! : ");
		i2c_printhex(TW_STATUS);
		uart_printstr("\r\n");
	}
	RESET(TWCR, TWSTA);
}

void	i2c_start_read()
{
	// set start condition
	SET(TWCR, TWSTA);
	SET(TWCR, TWINT);
	while (!(TWCR & (1<<TWINT)))
		;
	if (TW_STATUS != TW_START && TW_STATUS != TW_REP_START)
	{
		uart_printstr("ERROR! : ");
		i2c_printhex(TW_STATUS);
		uart_printstr("\r\n");
	}
	
	// set SLA+R mode
	TWDR = (0x38 << 1) + 1; // set ATH20 slave address
	SET(TWCR, TWINT); // clear flag

	while (!(TWCR & (1<<TWINT)))
		;
	if (TW_STATUS != TW_MR_SLA_ACK)
	{
		uart_printstr("ERROR! : ");
		i2c_printhex(TW_STATUS);
		uart_printstr("\r\n");
	}
	RESET(TWCR, TWSTA);
	SET(TWCR, TWEA);
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
		uart_printstr("ERROR!\r\n");
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
		uart_printstr("ERROR!\r\n");
	unsigned char c = TWDR;
	return c;
}

void	print_hex_value()
{
	for (uint8_t i = 0; i<6; i++) {
		while (!(TWCR & (1<<TWINT)))
			;
		if (i == 0 && (TWDR & 0x80) == 0x80)
		{
			i--;
			continue;
		}
		i2c_printhex(TWDR);
		uart_printstr(" ");
		SET(TWCR, TWEA);
		SET(TWCR, TWINT);
	}
	uart_printstr("\r\n");
	RESET(TWCR, TWEA);
	SET(TWCR, TWINT);
}

void	calibrate()
{
	i2c_start_read();
	_delay_ms(100);
	if ((i2c_read(false) & 0b1000) != 0b1000) // need calibration
	{
		uart_printstr("Calibrating...\r\n");
		i2c_stop();
		_delay_ms(100);
		i2c_start_write();
		_delay_ms(100);
		i2c_write(0xBE);
		i2c_write(0x08);
		i2c_write(0x00);
	}
	i2c_stop();
	_delay_ms(100);
	i2c_start_write();
}

int	main()
{
	i2c_init();
	uart_init();

	_delay_ms(100);
	calibrate();
	_delay_ms(100);
	i2c_start_write();
	_delay_ms(100);

	while (1)
	{
		i2c_write(0xAC);
		i2c_write(0x33);
		i2c_write(0x00);
		_delay_ms(100);
		i2c_stop();
		_delay_ms(100);
		i2c_start_read();
		print_hex_value();
		_delay_ms(100);
		i2c_stop();
		_delay_ms(100);
		i2c_start_write();
		_delay_ms(100);
	}
	i2c_stop();
}