#include <avr/io.h>
#include "SPI_Slave.h"

void SPI_SlaveInit(void)
{
	DDRB |= (1<<PB4); // MISO como salida
	SPCR = (1<<SPE);  // Habilitar SPI en modo esclavo
}

uint8_t SPI_Transfer(uint8_t data)
{
	SPDR = data; // Cargar dato a enviar
	while (!(SPSR & (1<<SPIF))); // Esperar transferencia
	return SPDR; // Retornar dato recibido
}
