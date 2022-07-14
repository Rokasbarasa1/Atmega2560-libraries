#include "./i2c_protocol_driver.h"

uint8_t initialized = 0;

void init_i2c(){
    if(initialized){
        return;
    }

    // Set SDA and SCL to input with pullup resistors
	PORTD |= _BV(PD0) | _BV(PD1);
	DDRD  |= _BV(PD0) | _BV(PD1);
	DDRD  &= ~(_BV(PD0) | _BV(PD1));
	
    // set prescaler to 1
    TWSR =  ~(_BV(TWPS0) | _BV(TWPS1));

    // SCL freq = F_CPU / (16 + 2*(TWBR) * 4^presaler )
    // default SCL should be 100,000 
    TWBR = ((F_CPU / 100000) - 16) / 2;

    // Enable i2c, enable interrupts, enable acknowledge bit 
    // TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA);

    // Enable interrupts
    TWCR = _BV(TWIE);

    // Clear the interrupt bit
    TWCR = _BV(TWINT);    
}

void i2c_start(){
    // reset interrupt flag, enable i2c, start bit
    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTA);

    // Wait for interrupt to bit to be set
    while(!(TWCR & _BV(TWINT)));
}

/**
 * Sends the slave address and the action type
 * Read - 1
 * Write - 0
 */
void i2c_slave_address_action(uint8_t address, uint8_t action){
    if(action <= 1){
        // declare writing or reading with slave address
        TWDR = (address << 1) | action;

        // Send the data
        TWCR = (1 << TWINT) | (1 << TWEN);
        while(!(TWCR & _BV(TWINT)));
    }
}

void i2c_write(uint8_t data){
    // Set data to the register
    TWDR = data;

    // This will start the writing
    TWCR = _BV(TWINT) | _BV(TWEN);

    // write the data
    while(!(TWCR & _BV(TWINT)));
}

void i2c_stop(){
    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);

    // while(!(TWCR & _BV(TWINT)));
}

uint8_t i2c_read(uint8_t ack){
    if(ack){
        TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
    }else{
        TWCR = _BV(TWINT) | _BV(TWEN);
    }

    while(!(TWCR & _BV(TWINT)));

    return TWDR;
}


void i2c_master_write(uint8_t address, uint8_t* data, uint8_t data_length, uint8_t stop_transmission){
    // Start the operation
    i2c_start();
    // Select slave and specify action
    i2c_slave_address_action(address, 0);
    
    // Write all the data
    for(uint8_t i = 0; i < data_length; i++){
        i2c_write(data[i]);
    }

    // Stop the operation or dont if repeated start is needed
    if(stop_transmission){
        i2c_stop();
    }
}

void i2c_master_read(uint8_t address, uint8_t* returned_data, uint8_t quantity){
    // Start the operation
    i2c_start();
    // Select slave and specify action
    i2c_slave_address_action(address, 1);
    
    // Read all the data
    for(uint8_t i = 0; i < quantity-1; i++){
        // Read data and acknowledge 
        returned_data[i] = i2c_read(1);
    }
    // Read last byte with no ack
    returned_data[quantity-1] = i2c_read(0);
    
    // Stop the operation
    i2c_stop();
}