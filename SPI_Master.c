#include <avr/io.h>
#include <util/delay.h>
#include "SPI_Master.h"

#define SS_PIN PB2

void SPI_MasterInit(void) {
	DDRB |= (1<<PB3)|(1<<PB5)|(1<<SS_PIN);
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	PORTB |= (1<<SS_PIN);
}

uint8_t SPI_Transfer(uint8_t data) {
	SPDR = data;
	while (!(SPSR & (1<<SPIF)));
	return SPDR;
}

uint16_t SPI_ReadADC(uint8_t canal) {
	uint8_t msb, lsb;
	PORTB &= ~(1<<SS_PIN);
	_delay_ms(2);
	SPI_Transfer(canal);
	_delay_ms(2);
	msb = SPI_Transfer(0x00);
	_delay_ms(2);
	lsb = SPI_Transfer(0x00);
	PORTB |= (1<<SS_PIN);
	return ((uint16_t)msb << 8) | lsb;
}

