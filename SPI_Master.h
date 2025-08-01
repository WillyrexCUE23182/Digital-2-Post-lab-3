#ifndef SPI_MASTER_H
#define SPI_MASTER_H

#include <stdint.h>

void SPI_MasterInit(void);
uint8_t SPI_Transfer(uint8_t data);
uint16_t SPI_ReadADC(uint8_t canal);

#endif
