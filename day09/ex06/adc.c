#include "main.h"

void	init_ADC()
{
	//enable ADC
	SET(ADCSRA, ADEN);

	// mode 10 bit
	RESET(ADMUX, ADLAR);

	// set voltage reference to AVcc
	RESET(ADMUX, REFS1);
	SET(ADMUX, REFS0);

	// prescaler
	SET(ADCSRA, ADPS2);
	SET(ADCSRA, ADPS1);
	SET(ADCSRA, ADPS0);
}
