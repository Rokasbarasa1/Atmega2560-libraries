#pragma once

#include <avr/io.h>


// Needs 3.6V
// Needs to be connected to i2c 

void init_accelerometer(uint8_t spi);
void get_axis_readings(int16_t* data, uint8_t spi);
