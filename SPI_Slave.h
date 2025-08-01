#ifndef SPI_SLAVE_H
#define SPI_SLAVE_H

#include <stdint.h>

void SPI_SlaveInit(void);
uint8_t SPI_Transfer(uint8_t data);

#endif
