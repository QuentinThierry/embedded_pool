#include "main.h"

#define ONE_SEC_TIMER1 15625

void	uart_init()
{
	const uint16_t	baud = F_CPU / (UART_BAUDRATE * 16);
	UBRR0H = (baud & 0xff00) >> 8; // set baudrate
	UBRR0L = baud & 0xff;
	SET(UCSR0B, TXEN0); // set USART mode to receive


	RESET(UCSR0C, UMSEL01);
	RESET(UCSR0C, UMSEL00);

	RESET(UCSR0C, UPM00);
	RESET(UCSR0C, UPM01);

	RESET(UCSR0C, USBS0);

	SET(UCSR0C, UCSZ00);
	SET(UCSR0C, UCSZ01);
	RESET(UCSR0B, UCSZ02);
}

void	uart_printstr(const char *str)
{
	uint16_t	i = 0;
	while (str[i])
	{
		while (!(UCSR0A & (1 << UDRE0))); // wait data to be transmit
		UDR0 = str[i];
		i++;
	}
}

ISR(TIMER1_COMPA_vect)
{
	uart_printstr("Hello World!\r\n");
}

void	init_timer1()
{
	// prescaler -> doc : 16.11.2
	SET(TCCR1B, CS12); // set prescaling to F_CPU/1024 -> 15625 bit 1
	RESET(TCCR1B, CS11); // set prescaling to F_CPU/1024 -> 15625 bit 2
	SET(TCCR1B, CS10); // set prescaling to F_CPU/1024 -> 15625 bit 3

	// compare match -> doc : 16.11.1 - table 16-1
	SET(TCCR1A, COM1A1); // set toggle OC1A on compare match bit 1
	RESET(TCCR1A, COM1A0); // set toggle OC1A on compare match bit 2

	// set counter mode operation -> doc : 16.11.1 - table 16-4
	RESET(TCCR1B, WGM13); // set bit 3
	SET(TCCR1B, WGM12); // set bit 2
	RESET(TCCR1A, WGM11); // set bit 1
	RESET(TCCR1A, WGM10); // reset bit 0

	SET(TIMSK1, OCIE1A); // set interrupt to TIMER1_COMPA

	OCR1A = ONE_SEC_TIMER1 * 2;
}

int main()
{
	SET(SREG, 7);	// activate global interrupt
	uart_init();
	init_timer1();
	while (1);
}