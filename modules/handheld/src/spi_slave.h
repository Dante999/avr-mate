#ifndef SPI_SLAVE_H
#define SPI_SLAVE_H

#include <stdint.h>

void    spi_slave_init(void);
uint8_t spi_slave_transceive(uint8_t data);

#endif // SPI_SLAVE_H
