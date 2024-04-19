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

void	print_hex_value(uint8_t data)
{
	const char base[] = "0123456789ABCDEF";
	uart_printstr("0x");
	uart_tx(base[(data & 0xf0) >> 4]);
	uart_tx(base[(data & 0xf)]);
}

void	i2c_start_write()
{
	// set start condition
	// uart_printstr("Sending start in write mode\r\n");

	SET(TWCR, TWSTA);
	SET(TWCR, TWINT);
	while (!(TWCR & (1<<TWINT)))
		;
	if (TW_STATUS == TW_START || TW_STATUS == TW_REP_START)
		/* uart_printstr("OK!\r\n") */;
	else
	{
		uart_printstr("ERROR! : ");
		print_hex_value(TW_STATUS);
		uart_printstr("\r\n");
	};
	
	// uart_printstr("Sending slave address...\r\n");
	// set SLA+W/R mode
	TWDR = (0x38 << 1); // set ATH20 slave address
	SET(TWCR, TWINT); // clear flag

	while (!(TWCR & (1<<TWINT)))
		;
	if (TW_STATUS != TW_MT_SLA_ACK)
	{
		uart_printstr("ERROR! : ");
		print_hex_value(TW_STATUS);
		uart_printstr("\r\n");
	}
	else
		/* uart_printstr("OK!\r\n") */;
	RESET(TWCR, TWSTA);
}

void	i2c_start_read()
{
	// set start condition
	// uart_printstr("Sending start in read mode\r\n");

	SET(TWCR, TWSTA);
	SET(TWCR, TWINT);
	while (!(TWCR & (1<<TWINT)))
		;
	if (TW_STATUS == TW_START || TW_STATUS == TW_REP_START)
		/* uart_printstr("OK!\r\n") */;
	else
	{
		uart_printstr("ERROR! : ");
		print_hex_value(TW_STATUS);
		uart_printstr("\r\n");
	}
	
	// uart_printstr("Sending slave address...\r\n");
	// set SLA+W/R mode
	TWDR = (0x38 << 1) + 1; // set ATH20 slave address
	SET(TWCR, TWINT); // clear flag

	SET(TWCR, TWEA);
	while (!(TWCR & (1<<TWINT)))
		;
	if (TW_STATUS != TW_MR_SLA_ACK)
	{
		/* uart_printstr("ERROR! : ") */;
		print_hex_value(TW_STATUS);
		uart_printstr("\r\n");
	}
	else
		/* uart_printstr("OK!\r\n") */;
	RESET(TWCR, TWSTA);
}

void	i2c_stop()
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	uart_printstr("Stopping\r\n");
}

void	i2c_write(unsigned char data)
{
	TWDR = data;
	while (!(TWCR & (1<<TWINT)))
		;
	if (TW_STATUS != TW_MT_DATA_ACK)
		uart_printstr("ERROR!\r\n");
	SET(TWCR, TWINT);
}

void	i2c_read()
{
	for (uint8_t i = 0; i<6; i++) {
		while (!(TWCR & (1<<TWINT)))
			;
		print_hex_value(TWDR);
		uart_printstr(" ");
		SET(TWCR, TWINT);
		SET(TWCR, TWEA);
	}
	uart_printstr("\r\n");
	RESET(TWCR, TWEA);
	SET(TWCR, TWINT);
}

int	main()
{
	i2c_init();
	uart_init();

	_delay_ms(40);
	i2c_start_write();
	i2c_write(0x71);
	_delay_ms(150);
	i2c_start_read();
	i2c_read();
	// i2c_start_write();
	// i2c_write(0xBE);
	// i2c_write(0x08);
	// i2c_write(0x00);
	_delay_ms(150);

	
	while (1)
	{
		i2c_start_write();
		i2c_write(0xAC);
		i2c_write(0x33);
		i2c_write(0x00);
		_delay_ms(150);
		i2c_start_read();
		_delay_ms(1000);
		i2c_read();
		// SET(TWCR, TWEA);
		// _delay_ms(200);
		// uart_printstr(" : ");
		// print_hex_value((TW_STATUS));
		// uart_printstr("\r\n");
		_delay_ms(100);
	}
	// i2c_stop();
}