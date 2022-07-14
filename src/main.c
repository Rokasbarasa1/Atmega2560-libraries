#ifndef __AVR_ATmega2560__
    #define __AVR_ATmega2560__
#endif

#define F_CPU 16000000

#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <avr/interrupt.h>

#include "../lib/i2c_protocol_driver/i2c_protocol_driver.h" 
#include "../lib/status_led_driver/status_led_driver.h"
#include "../lib/uart_protocol_driver/uart_protocol_driver.h"
#include "../lib/twi_master/twi_master.h"

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

char* int_to_string(uint16_t number){
    // get digit count
    uint8_t count = 0;   // variable declaration 
    uint16_t number_temp = number; 
    while(number_temp!=0)  {  
        number_temp=number_temp/10;  
        count++;  
    }    

    char *string = malloc(count+1);
    
    for(uint8_t i = 0; i < count; i++){
        uint16_t divisor = 1;
        for(uint8_t j = 0; j < count-1-i; j++){
            divisor = divisor * 10;
        }

        string[i] = 48 + (number / divisor);
        number =  number % divisor;
    }

    string[count] = '\0';

    return string;
}

#define ADXL345 0x53

int main(void){
    sei();
    init_uart_protocol(0, 9600);
    {
    // init_uart_protocol(2, 115200);

    // uart_write_string(0, "Serial 0 \n", 0);
    // uart_write_string(0, binary_to_string(UBRR0H), 1);
    // uart_write_string(0, binary_to_string(UBRR0L), 1);
    // uart_write_string(0, " \n", 0);


    // uart_write_string(0, "\nSerial 2 \n");
    // uart_write_string(0, binary_to_string(UBRR2H));
    // uart_write_string(0, binary_to_string(UBRR2L));
    // uart_write_string(0, " \n");
    }
    uint8_t dataInit[] = {0x2D, 8};
    uint8_t data2[] = {0,0,0,0,0,0};
    uint8_t data1[] = {0x32};

    {
        // tw_init(TW_FREQ_100K, 1);
        // tw_master_transmit(ADXL345, dataInit, 2, 0);
    }

    uint16_t X_out, Y_out, Z_out;
    init_i2c();
    i2c_master_write(ADXL345, dataInit, 2, 1);

    while (1){ 
        i2c_master_write(ADXL345, data1, 2, 0);
        i2c_master_read(ADXL345, data2, 6);

        // tw_master_transmit(ADXL345, data1, 2, 1);
        // tw_master_receive(ADXL345, data2, 6);

        X_out = data2[0] | (data2[1] << 8);
        // X_out = X_out/256;
        Y_out = data2[2] | (data2[3] << 8);
        // Y_out = Y_out/256;
        Z_out = data2[4] | (data2[5] << 8);
        // Z_out = Z_out/256;

        // uart_write_string(0, int_to_string(X_out), 1);
        // uart_write_string(0, " \n", 0);
        // uart_write_string(0, int_to_string(Y_out), 1);
        // uart_write_string(0, " \n", 0);
        uart_write_string(0, int_to_string(Z_out), 1);
        uart_write_string(0, " \n", 0);

        uart_write_string(0, binary_to_string(Z_out>>8), 1);
        uart_write_string(0, binary_to_string(Z_out), 1);
        uart_write_string(0, " \n", 0);

        // uart_write_string(0, "X= ");
        // uart_write_string(0, X_out);
    
        // uart_write_string(0, "Y= ");
        // uart_write_string(0, Y_out);
        
        // uart_write_string(0, "Z= ");
        // uart_write_string(0, Z_out);

        // uart_write_string(0, buf);
        // uart_write_string(0, " \n");
        _delay_ms(1000);
        {
        // if (uart_read_ready(0)) {
        //     int inByte = uart_read_byte(0);
        //     uart_write_byte(2, inByte);
        // }
            
        // if (uart_read_ready(2)) {
        //     int inByte = uart_read_byte(2);
        //     uart_write_byte(0, inByte);
        // } 


        // if (uart_read_ready(0)) {
        //     int inByte = uart_read_byte(0);
        //     uart_send_byte(inByte);
        // }

        // if (uart_read_ready(2)) {
        //     int inByte = uart_read();
        //     uart_write_byte(0, inByte);
        // } 
        }
    }
    return 0;
}