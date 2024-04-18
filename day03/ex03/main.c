#include "main.h"

void	uart_init()
{
	const uint16_t	baud = F_CPU / (UART_BAUDRATE * 16);
	UBRR0H = (baud & 0xff00) >> 8; // set baudrate
	UBRR0L = baud & 0xff;
	SET(UCSR0B, TXEN0); // set USART mode to transmit
	SET(UCSR0B, RXEN0); // set USART mode to receive

	RESET(UCSR0C, UMSEL01); // set asynchronous USART mode
	RESET(UCSR0C, UMSEL00);

	RESET(UCSR0C, UPM00); // disable parity mode for 8N1
	RESET(UCSR0C, UPM01);

	RESET(UCSR0C, USBS0); // set 1 stop bit

	SET(UCSR0C, UCSZ00); // set 8 bit size
	SET(UCSR0C, UCSZ01);
	RESET(UCSR0B, UCSZ02);

	SET(UCSR0B, RXCIE0); // activate RX interrupt
}

void	uart_tx(char c)
{
	while (!(UCSR0A & (1 << UDRE0))); // wait data to be transmit
	UDR0 = c;
}

char	uart_rx(void)
{
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}

char	to_lower(char c)
{
	if (c >= 'a')
		c -= ('a' - 'A');
	else if (c >= 'A')
		c += ('a' - 'A');
	return c;
}

ISR(USART_RX_vect)
{
	char c = uart_rx();
	uart_tx(to_lower(c));
}

int main()
{
	SET(SREG, 7);	// activate global interrupt
	uart_init();
	while (1)
	{
		
	}
}