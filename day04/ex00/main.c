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

void	i2c_start()
{
	// set start condition
	uart_printstr("Sending start...\r\n");
	SET(TWCR, TWINT);
	SET(TWCR, TWSTA);
	while (!(TWCR & (1<<TWINT)))
		;
	if (TW_STATUS != TW_START)
		uart_printstr("ERROR!");
	else
		uart_printstr("OK!\r\n");

	uart_printstr("Sending slave address...\r\n");

	// set SLA+W mode
	TWDR = (0x38 << 1); // set ATH20 slave address
	SET(TWCR, TWINT); // clear flag

	while (!(TWCR & (1<<TWINT)))
		;
	if (TW_STATUS != TW_MT_SLA_ACK)
		uart_printstr("ERROR!\r\n");
	else
		uart_printstr("OK!\r\n");
}

void	i2c_stop()
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	uart_printstr("Stopping\r\n");
}

int	main()
{
	i2c_init();
	uart_init();

	i2c_start();
	i2c_stop();
	while (1);
}