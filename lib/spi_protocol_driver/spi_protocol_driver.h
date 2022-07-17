#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>

void init_spi(uint8_t mode, uint8_t most_significant);
void spi_set_mode(uint8_t mode);
void spi_set_significance(uint8_t most_significant);
void spi_close();

void spi_write_byte(uint8_t data);
uint8_t spi_read_byte();

void spi_read_master(uint8_t* returned_data, uint8_t quantity);
void spi_write_master(uint8_t* data, uint8_t data_length);