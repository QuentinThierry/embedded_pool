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
	while (!(UCSR0A & (1 << UDRE0))); // wait data to be ready to transmit
	UDR0 = c;
}

char	uart_rx(void)
{
	while (!(UCSR0A & (1 << RXC0))); // wait data to be ready to read
	return UDR0;
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

volatile static const char g_user[] = "qthierry";
volatile static const char g_pass[] = "outstanding";

void	do_on_delete(bool is_username, volatile uint8_t *nb_error_user, volatile uint8_t *nb_error_pass, volatile uint8_t *cursor)
{
	if (is_username && *nb_error_user > 0)
		(*nb_error_user)--;
	if (!is_username && *nb_error_pass > 0)
		(*nb_error_pass)--;
	if (*cursor > 0)
	{
		(*cursor)--;
		uart_tx('\b');
		uart_tx(' ');
		uart_tx('\b');
	}
}

void	check_error_user(uint8_t cursor, char c, volatile uint8_t *nb_error_user)
{
	if (cursor > sizeof(g_user) || g_user[cursor] != c)
	{
		(*nb_error_user)++;
		return ;
	}
}

void	check_error_pass(uint8_t cursor, char c, volatile uint8_t *nb_error_pass)
{
	if (cursor > sizeof(g_pass) || g_pass[cursor] != c)
	{
		(*nb_error_pass)++;
		return ;
	}
}

#define NB_LED_TURN 5
#define NB_LED_FLASH 15
void	flash_lights(bool is_ok)
{
	RESET(PORTD, PD6);
	RESET(PORTD, PD5);
	uint8_t	i = 0;
	while (i < NB_LED_TURN)
	{
		TOGGLE(PORTB, PB0);
		_delay_ms(50);
		TOGGLE(PORTB, PB1);
		_delay_ms(50);
		TOGGLE(PORTB, PB2);
		_delay_ms(50);
		TOGGLE(PORTB, PB4);
		_delay_ms(50);
		TOGGLE(PORTB, PB0);
		_delay_ms(50);
		TOGGLE(PORTB, PB1);
		_delay_ms(50);
		TOGGLE(PORTB, PB2);
		_delay_ms(50);
		TOGGLE(PORTB, PB4);
		_delay_ms(50);
		i++;
	}
	RESET(PORTB, PB0);
	RESET(PORTB, PB1);
	RESET(PORTB, PB2);
	RESET(PORTB, PB4);
	i = 0;
	while (i < NB_LED_FLASH * 2)
	{
		TOGGLE(PORTB, PB0);
		TOGGLE(PORTB, PB1);
		TOGGLE(PORTB, PB2);
		TOGGLE(PORTB, PB4);
		i++;
		_delay_ms(50);
	}
	if (is_ok)
		SET(PORTD, PD6);
	else
		SET(PORTD, PD5);
	_delay_ms(100);
}

void	display_end(bool is_ok)
{
	uart_printstr("\r\nProcessing...\r\n");
	flash_lights(is_ok);
	if (is_ok)
	{
		uart_printstr("\r\nBienvenue à toi, ");
		uart_printstr((const char *)g_user);
		uart_printstr(".\r\n");
	}
	else
	{
		uart_printstr("\r\nMauvais mot de passe.\r\n");
	}
}

ISR(USART_RX_vect)
{
	volatile static bool	is_username = true;
	volatile static uint8_t	cursor = 0;
	volatile static uint8_t	nb_error_user = 0;
	volatile static uint8_t	nb_error_pass = 0;

	char c = uart_rx();
	if (c == 127)
	{
		do_on_delete(is_username, &nb_error_user, &nb_error_pass, &cursor);// remove 1 to cursor and error + delete char
		return ;
	}
	if (is_username && c == '\r')
	{
		if (cursor != sizeof(g_user) - 1)
			nb_error_user = 1;
		cursor = 0;
		uart_printstr("\r\npassword : \r\n");
		is_username = false;
		return ;
	}
	else if (!is_username && c == '\r')
	{
		display_end(nb_error_user == 0 && nb_error_pass == 0 && cursor == sizeof(g_pass) - 1);
		is_username = true;
		cursor = nb_error_pass = nb_error_user = 0;
		uart_printstr("\r\nusername : \r\n");
		return ;
	}
	if (is_username)
	{
		check_error_user(cursor, c, &nb_error_user);
		uart_tx(c);
	}
	else
	{
		check_error_pass(cursor, c, &nb_error_pass);
		uart_tx('*');
	}
	if (cursor == UINT8_MAX)
	{
		is_username = true;
		cursor = nb_error_pass = nb_error_user = 0;
		uart_printstr("Too much characters, reseting...\r\nusername : \r\n");
		return ;
	}
	cursor++;
}

int main()
{
	SET(SREG, 7);	// activate global interrupt
	uart_init();
	uart_printstr("username : \r\n");
	while (1);
}