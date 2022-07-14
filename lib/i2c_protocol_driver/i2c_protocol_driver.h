#pragma once

#ifndef UNIT_TEST
    #include <avr/io.h>
    #include <avr/interrupt.h>
#else
    #include "../../extras/forced_include.h"
    #include "../../extras/include/avr/io.h"
#endif

void init_i2c();
void i2c_start();
void i2c_slave_address_action(uint8_t address, uint8_t action);
void i2c_write(uint8_t data);
void i2c_stop();
uint8_t i2c_read(uint8_t ack);
void i2c_master_write(uint8_t address, uint8_t* data, uint8_t data_length, uint8_t stop_transmission);
void i2c_master_read(uint8_t address, uint8_t* returned_data, uint8_t quantity);