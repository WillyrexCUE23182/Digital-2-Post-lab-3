/*
 * main.c
 * 
 * Author: Willy Ulises Cuellar
 * Description: Maestro SPI con menú UART. Lee ADC remoto y controla LEDs.
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>

#include "UART.h"
#include "SPI_Master.h"

/*********** Prototipos adicionales ***********/
void LED_Init(void);
void LED_ShowByte(uint8_t val);

/*********** Función principal ***********/
int main(void)
{
    char buf[64];
    UART_Init();
    SPI_MasterInit();
    LED_Init();

    while (1)
    {
        UART_TransmitString("Mande P para leer potenciometros o L para enviar valor 0–255:\r\n");
        uint8_t opcion = UART_ReceiveByte();
        while (!(UCSR0A & (1<<UDRE0))); UDR0 = opcion;

        if (opcion == 'P' || opcion == 'p')
        {
            uint16_t raw1 = SPI_ReadADC(6);
            uint16_t raw2 = SPI_ReadADC(7);
            uint32_t mv1 = ((uint32_t)raw1 * 5000 + 511) / 1023;
            uint32_t mv2 = ((uint32_t)raw2 * 5000 + 511) / 1023;

            snprintf(buf, sizeof(buf), "\r\nP1 raw=%u, P2 raw=%u; P1=%lu mV, P2=%lu mV\r\n", raw1, raw2, mv1, mv2);
            UART_TransmitString(buf);
        }
        else if (opcion == 'L' || opcion == 'l')
        {
            UART_TransmitString("\r\nValor: ");
            uint8_t digits[3] = {0};
            uint8_t i = 0;
            while (i < 3)
            {
                uint8_t c = UART_ReceiveByte();
                if (c == '\r' || c == '\n') break;
                if (c >= '0' && c <= '9')
                {
                    digits[i++] = c - '0';
                    while (!(UCSR0A & (1<<UDRE0))); UDR0 = c;
                }
            }

            uint16_t val = 0;
            if (i == 1) val = digits[0];
            else if (i == 2) val = digits[0]*10 + digits[1];
            else if (i == 3) val = digits[0]*100 + digits[1]*10 + digits[2];
            if (val > 255) val = 255;
            uint8_t user_val = (uint8_t)val;

            snprintf(buf, sizeof(buf), "\r\nValor recibido: %u\r\n", user_val);
            UART_TransmitString(buf);

            LED_ShowByte(user_val);

            PORTB &= ~(1<<PB2);
            _delay_ms(2);
            SPI_Transfer(255);
            _delay_ms(2);
            SPI_Transfer(user_val);
            PORTB |= (1<<PB2);
        }

        UART_TransmitString("\r\n");
    }
}

/*********** Subrutinas locales ***********/
void LED_Init(void) {
    DDRD |= 0b11111100;
    DDRB |= (1<<PB0)|(1<<PB1);
}

void LED_ShowByte(uint8_t val) {
    PORTD &= 0b00000011;
    PORTD |= (val & 0b00111111) << 2;
    PORTB &= 0b11111100;
    PORTB |= (val >> 6) & 0x03;
}
