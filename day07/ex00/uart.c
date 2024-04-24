#include "main.h"

void	uart_init()
{
	const uint16_t	baud = F_CPU / (UART_BAUDRATE * 16);
	UBRR0H = (baud & 0xff00) >> 8; // set baudrate
	UBRR0L = baud & 0xff;
	SET(UCSR0B, TXEN0); // set USART mode to transmit
	SET(UCSR0B, RXEN0); // set USART mode to recieve

	RESET(UCSR0C, UMSEL01); // set asynchronous USART mode
	RESET(UCSR0C, UMSEL00);

	RESET(UCSR0C, UPM00); // disable parity mode for 8N1
	RESET(UCSR0C, UPM01);

	RESET(UCSR0C, USBS0); // set 1 stop bit

	SET(UCSR0C, UCSZ00); // set 8 bit size
	SET(UCSR0C, UCSZ01);
	RESET(UCSR0B, UCSZ02);
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

void	uart_printhex(uint8_t data)
{
	const char base[] = "0123456789ABCDEF";
	// uart_printstr("0x");
	uart_tx(base[(data & 0xf0) >> 4]);
	uart_tx(base[(data & 0xf)]);
}

void	uart_printhex_word(uint16_t data)
{
	const char base[] = "0123456789ABCDEF";
	// uart_printstr("0x");
	uart_tx(base[(data & 0xf000) >> 12]);
	uart_tx(base[(data & 0xf00) >> 8]);
	uart_tx(base[(data & 0xf0) >> 4]);
	uart_tx(base[(data & 0xf)]);
}

char	uart_rx(void)
{
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}

char	uart_rtx()
{
	char c = uart_rx();
	uart_tx(c);
	return c;
}
