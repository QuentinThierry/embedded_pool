#include "main.h"

void	init_ADC()
{
	//enable ADC
	SET(ADCSRA, ADEN);

	// mode 8 bit
	SET(ADMUX, ADLAR);

	// set voltage reference to 1.1V
	RESET(ADMUX, REFS1);
	SET(ADMUX, REFS0);

	// prescaler
	SET(ADCSRA, ADPS2);
	SET(ADCSRA, ADPS1);
	SET(ADCSRA, ADPS0);
}
