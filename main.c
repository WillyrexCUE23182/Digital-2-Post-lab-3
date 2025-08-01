/*
 * main.c (esclavo)
 * 
 * Author: Willy Ulises Cuellar
 * Description: Esclavo SPI que recibe valor y devuelve lecturas ADC.
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#include "SPI_Slave.h"
#include "ADC.h"

int main(void)
{
    SPI_SlaveInit();
    ADC_Init();
    DDRD = 0xFF; // Mostrar valor recibido en LEDs (opcional)
    uint8_t canal = 0xFF;

    while (1)
    {
        uint8_t dato = SPI_Transfer(0x00);

        if (dato == 255)
        {
            uint8_t val = SPI_Transfer(0x00);
            PORTD = val; // Mostrar en LEDs
        }
        else if (dato == 6 || dato == 7)
        {
            canal = dato;
            uint16_t lectura = ADC_Read(canal);
            SPI_Transfer((lectura >> 8) & 0xFF); // MSB
            _delay_ms(2);
            SPI_Transfer(lectura & 0xFF);       // LSB
        }
    }
}




