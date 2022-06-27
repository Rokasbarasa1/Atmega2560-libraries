#ifndef __AVR_ATmega2560__
    #define __AVR_ATmega2560__
#endif

#define F_CPU 16000000

#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

#include "../lib/status_led_driver/status_led_driver.h"
#include "../lib/uart_protocol_driver/uart_protocol_driver.h"

char* binary_to_string(uint8_t binary){
    char *string = malloc(9);
    for(uint8_t i = 0; i < 8; i++){
        uint8_t bitStatus = (binary >> (i)) & 1;
        if (bitStatus){
            string[7-i] = '1';
        }else{
            string[7-i] = '0';
        }
    }
    string[8] = '\0';

    return string;
}

int main(void){
    init_status_led();
    init_uart_protocol(0, 9600);
    init_uart_protocol(2, 115200);

    uart_write_string(0, "Serial 0 \n");
    uart_write_string(0, binary_to_string(UBRR0H));
    uart_write_string(0, binary_to_string(UBRR0L));


    uart_write_string(0, "\nSerial 2 \n");
    uart_write_string(0, binary_to_string(UBRR2H));
    uart_write_string(0, binary_to_string(UBRR2L));
    // uart_write_string(0, " \n");

    // uart_write_string(2,"AT\r\n\0");

    // Event loop
    while (1){   
        if (uart_read_ready(0)) {
            int inByte = uart_read_byte(0);
            uart_write_byte(2, inByte);
        }
            
        if (uart_read_ready(2)) {
            int inByte = uart_read_byte(2);
            uart_write_byte(0, inByte);
        } 


        // if (uart_read_ready(0)) {
        //     int inByte = uart_read_byte(0);
        //     uart_send_byte(inByte);
        // }

        // if (uart_read_ready(2)) {
        //     int inByte = uart_read();
        //     uart_write_byte(0, inByte);
        // } 

    }
    return 0;
}