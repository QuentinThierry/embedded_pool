#include "main.h"




void	init_ADC()
{

	//enable ADC
	SET(ADCSRA, ADEN);

	// mode 8 bit
	SET(ADMUX, ADLAR);

	// select channel
	RESET(ADMUX, MUX3); // ADC0
	RESET(ADMUX, MUX2);
	RESET(ADMUX, MUX1);
	RESET(ADMUX, MUX0);

	// set voltage reference to AVcc
	RESET(ADMUX, REFS1);
	SET(ADMUX, REFS0);

	// prescaler
	SET(ADCSRA, ADPS2);
	SET(ADCSRA, ADPS1);
	SET(ADCSRA, ADPS0);
}



int	main()
{
	uart_init();

	init_ADC();
	while (1)
	{
		SET(ADCSRA, ADSC);
		while ((ADCSRA & (1 << ADSC)) != 0)
			;
		uart_printhex(ADCH);
		uart_printstr("\r\n");
		_delay_ms(20);
	}
}
