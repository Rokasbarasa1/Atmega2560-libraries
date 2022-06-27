#include "./status_led_driver.h"

void init_status_led(){
    DDRB |= _BV(DDB7);
    PORTB &= ~_BV(PB7);
}

void status_led_off(){
    PORTB &= ~_BV(PB7);
}

void status_led_on(){
    PORTB |= _BV(PB7);
}