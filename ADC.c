#include <avr/io.h>
#include "ADC.h"

void ADC_Init(void)
{
	ADMUX = (1<<REFS0); // Referencia AVcc, canal 0 por defecto
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1); // Habilita ADC, prescaler 64
}

uint16_t ADC_Read(uint8_t canal)
{
	ADMUX = (ADMUX & 0xF0) | (canal & 0x0F); // Seleccionar canal
	ADCSRA |= (1<<ADSC); // Iniciar conversión
	while (ADCSRA & (1<<ADSC)); // Esperar fin
	return ADC; // Leer resultado (ADCL + ADCH)
}
