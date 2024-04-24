#include "main.h"

void	init_ADC()
{

	//enable ADC
	SET(ADCSRA, ADEN);

	// mode 10 bit
	RESET(ADMUX, ADLAR);

	// set voltage reference to 1.1V
	SET(ADMUX, REFS1);
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
	ADMUX |= val & 0xf;
}

void	print_val()
{
	SET(ADCSRA, ADSC);
	while ((ADCSRA & (1 << ADSC)) != 0)
		;
	uint16_t	val = ADC;

	val = (val * 25) / 314;

	uart_int16(val);
}


int	main()
{
	uart_init();

	init_ADC();
	_delay_ms(50);

	while (1)
	{
		select_channel(8);
		print_val();
		uart_printstr("\r\n");
		_delay_ms(20);
	}
}
