#pragma once

#ifndef UNIT_TEST
    #include <avr/io.h>
    #include <util/delay.h>
    #include <avr/interrupt.h>
#else
    #include "../../extras/forced_include.h"
    #include "../../extras/include/avr/io.h"
    #include "../../extras/include/util/delay.h"
#endif

void init_uart_protocol(uint8_t serial_device, uint32_t baud_rate);

uint8_t uart_write_ready(uint8_t serial_device);
uint8_t uart_read_ready(uint8_t serial_device);

uint8_t uart_read_byte(uint8_t serial_device);
void uart_write_byte(uint8_t serial_device, uint8_t data);
void uart_write_string(uint8_t serial_device, char *string);

// void uart_send_byte(uint8_t c);
// uint8_t uart_read(void);