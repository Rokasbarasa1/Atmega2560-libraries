#include "./spi_protocol_driver.h"
#include "../uart_protocol_driver/uart_protocol_driver.h"

void init_spi(uint8_t mode, uint8_t most_significant){
    sei();

    // CS = PB0 - DISABLED and not used
    // SCK = PB1
    // MOSI  = PB2
    // MISO  = PB3

    // configure the pins that SPI uses, MOSI, MISO, SCK, Slave select will be done later
    DDRB |= _BV(DDB3) | _BV(DDB1) | _BV(DDB2) | _BV(DDB0);
    PORTB |= _BV(PB3) | _BV(PB1) | _BV(PB2) ;//| _BV(PB0);



    // These bits control the clock of SPI:
    // This combination leads to a frequency of:
    // 16Mhz/ 128 - not very fast
    SPCR |= _BV(SPR1) | _BV(SPR0);    
    
    spi_set_mode(mode);
    spi_set_significance(most_significant);
    // Enable spi, set master, enable interrupt
    SPCR |= _BV(SPE) | _BV(MSTR);// | _BV(SPIE);
}

// Set clock polarity and clock phase
void spi_set_mode(uint8_t mode){
    if(mode == 0){
        SPCR &= ~ (_BV(CPOL) | _BV(CPHA));
    }else if(mode == 1){
        SPCR |= _BV(CPHA);
        SPCR &= ~ (_BV(CPOL));
    }else if(mode == 2){
        SPCR |= _BV(CPOL);
        SPCR &= ~ (_BV(CPHA));
    }else if(mode == 3){
        SPCR |= _BV(CPOL) | _BV(CPHA);
    }
}

void spi_set_significance(uint8_t most_significant){
    // Set from which bit the data is transferred form. 
    // This is when SPI has the whole byte of data l
    // added into the register already
    // set bit means least significant bit.
    if(most_significant){
        SPCR &= ~ (_BV(DORD));
    }else if(!most_significant){
        SPCR |= _BV(DORD); 
    }
}
    
void spi_write_byte(uint8_t data){
    SPDR = data;
    while(!(SPSR & (1<<SPIF)));
}

uint8_t spi_read_byte(){
    // Send some nonsense like all 1's
    SPDR = 0xFF;
    while(!(SPSR & (1<<SPIF)));

    return SPDR;
}

void spi_close(){
    SPCR &= ~ (_BV(SPE));
}

void spi_read_master(uint8_t* returned_data, uint8_t quantity){
    for(uint8_t i = 0; i < quantity; i++){
        // Send some nonsense like all 1's
        SPDR = 0xFF;
        while(!(SPSR & (1<<SPIF)));
        returned_data[i] = SPDR;
    }
}

void spi_write_master(uint8_t* data, uint8_t data_length){
    for(uint8_t i = 0; i < data_length; i++){
        SPDR = data[i];
        while(!(SPSR & (1<<SPIF)));
    }
}

// void spi_write_manual(){

// }


// void spi_write_manual(){
    
// }