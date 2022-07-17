#include "./i2c_protocol_driver.h"

#include "../uart_protocol_driver/uart_protocol_driver.h"
#include <util/twi.h>

uint8_t initialized = 0;
#define OK 0

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

    // Enable interrupts
    TWCR = _BV(TWIE);

    // Clear the interrupt bit
    TWCR = _BV(TWINT);    

    sei();

    initialized = 1;
}

uint8_t i2c_start(){
    // reset interrupt flag, enable i2c, start bit
    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTA);

    // Wait for interrupt to bit to be set
    while(!(TWCR & _BV(TWINT))){}

    	/* Check error */
	if (TW_STATUS != TW_START && TW_STATUS != TW_REP_START)
	{
		return TW_STATUS;
	}

    return OK;
}

/**
 * Sends the slave address and the action type
 * Read - 1
 * Write - 0
 */
uint8_t i2c_slave_address_action(uint8_t address, uint8_t action){
    if(action <= 1){
        // declare writing or reading with slave address
        TWDR = (address << 1) | action;

        // Send the data
        TWCR = (1 << TWINT) | (1 << TWEN);
        while(!(TWCR & _BV(TWINT))){}

        if (TW_STATUS != TW_MT_SLA_ACK && TW_STATUS != TW_MR_SLA_ACK){
		    return TW_STATUS;
	    }

	    return OK;
    }
    return 1;
}

uint8_t i2c_write(uint8_t data){
    // Set data to the register
    TWDR = data;

    // This will start the writing
    TWCR = _BV(TWINT) | _BV(TWEN);

    // write the data
    while(!(TWCR & _BV(TWINT))){}
    
	if (TW_STATUS != TW_MT_DATA_ACK){
		return TW_STATUS;
	}

	return OK;
}

void i2c_stop(){
    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);
}

uint8_t i2c_read(uint8_t ack, uint8_t* data){
    if(ack){
        TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
        while(!(TWCR & _BV(TWINT))){}

        if (TW_STATUS != TW_MR_DATA_ACK){
            return TW_STATUS;
        }
    }else{
        TWCR = _BV(TWINT) | _BV(TWEN);
        while(!(TWCR & _BV(TWINT))){}

        if (TW_STATUS != TW_MR_DATA_NACK){
            return TW_STATUS;
        }
    }

    data[0] = TWDR;

    return OK;
}


uint8_t i2c_master_write(uint8_t address, uint8_t* data, uint8_t data_length, uint8_t stop_transmission){
    uint8_t status = 0;
    // Start the operation
    status = i2c_start();
    if(status != OK){
        return status;
    }
    // Select slave and specify action
    status = i2c_slave_address_action(address, 0);
    if(status != OK){
        return status;
    }
    // Write all the data
    for(uint8_t i = 0; i < data_length; i++){
        status = i2c_write(data[i]);
        if(status != OK){
            return status;
        }
    }

    // Stop the operation or dont if repeated start is needed
    if(stop_transmission){
        i2c_stop();
    }

    return OK;
}

uint8_t i2c_master_read(uint8_t address, uint8_t* returned_data, uint8_t quantity){
    uint8_t status = 0;

    // Start the operation
    status = i2c_start();
	if(status != OK){
        return status;
    }

    // Select slave and specify action
    status = i2c_slave_address_action(address, 1);
    if(status != OK){
        return status;
    }

    // Read all the data
    uint8_t data[] = {0};
    for(uint8_t i = 0; i < quantity-1; i++){
        // Read data and acknowledge 
        status = i2c_read(1, data);
        returned_data[i] = data[0];
        if(status != OK){
            return status;
        }
    }

    // Read last byte with no ack
    status = i2c_read(0, data);
    returned_data[quantity-1] = data[0];

    if(status != OK){
        return status;
    }


    // Stop the operation
    i2c_stop();

    return OK;
}