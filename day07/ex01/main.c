#include "main.h"




void	init_ADC()
{

	//enable ADC
	SET(ADCSRA, ADEN);

	// mode 8 bit
	SET(ADMUX, ADLAR);

	// set voltage reference to AVcc
	RESET(ADMUX, REFS1);
	SET(ADMUX, REFS0);

	// prescaler
	SET(ADCSRA, ADPS2);
	SET(ADCSRA, ADPS1);
	SET(ADCSRA, ADPS0);
}

void	select_channel(uint8_t val)
{
	// select channel
	ADMUX &= 0xf0;
	ADMUX |= val & 0x3;
}

void	print_val()
{
	SET(ADCSRA, ADSC);
	while ((ADCSRA & (1 << ADSC)) != 0)
		;
	uart_printhex(ADCH);
}


int	main()
{
	uart_init();

	init_ADC();
	while (1)
	{
		select_channel(0);
		print_val();
		uart_printstr(" ");
		select_channel(1);
		print_val();
		uart_printstr(" ");
		select_channel(2);
		print_val();
		uart_printstr(" ");
		_delay_ms(20);
		uart_printstr("\r\n");
	}
}
