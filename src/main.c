#ifndef __AVR_ATmega2560__
    #define __AVR_ATmega2560__
#endif

#define F_CPU 16000000

#include <util/delay.h>

// #include "../lib/status_led_driver/status_led_driver.h"
#include "../lib/uart_protocol_driver/uart_protocol_driver.h"
// #include "../lib/twi_master/twi_master.h"
#include "../lib/utils/utils.h"
#include "../lib/adxl345_accelerometer_driver/adxl345_accelerometer_driver.h"

int main(void){
    init_uart_protocol(0, 9600);
    uart_write_string(0, "Starting test run\n\n", 0);
    
    init_accelerometer(1);
    int16_t data[] = {0,0,0};
    uart_write_string(0, "Accel setup\n", 0);

    while (1){ 
        get_axis_readings(data, 1);

        uart_write_string(0, "X= ", 0);
        uart_write_string(0, float_to_string(data[0]), 1);
        uart_write_string(0, " Y= ", 0);
        uart_write_string(0, float_to_string(data[1]), 1);
        uart_write_string(0, " Z= ", 0);
        uart_write_string(0, float_to_string(data[2]), 1);
        uart_write_string(0, " \n", 0);
        // _delay_ms(1000);
    }
    return 0;
}