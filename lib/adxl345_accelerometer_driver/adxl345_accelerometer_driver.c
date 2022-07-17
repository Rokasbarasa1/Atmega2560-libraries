
#include "./adxl345_accelerometer_driver.h"

#include "../i2c_protocol_driver/i2c_protocol_driver.h"
#include "../uart_protocol_driver/uart_protocol_driver.h"
#include "../spi_protocol_driver/spi_protocol_driver.h"

#define ADXL345 0x53

void init_accelerometer(uint8_t spi){
    uint8_t dataInit[] = {0x2D, 8};
    uint8_t x_calibration[] = {0x1E, 0};
    uint8_t y_calibration[] = {0x1F, 0};
    uint8_t z_calibration[] = {0x20, 0b11111101};

    if(spi){
        // set up slave select 
        DDRH |= _BV(PH6);
        init_spi(3, 1);

        // spi_set_mode(3);
        // spi_set_significance(1);
        
        // select slave
        PORTH &= ~(_BV(PH6));
        // set write bit and whatever MB means in the data sheet
        dataInit[0] |= _BV(6);
        dataInit[0] &= ~(_BV(7));
        spi_write_master(dataInit,2);

        // deselect slave
        PORTH |= _BV(PH6);

        // Offset calibration - writing to registers on the ADXL345

        // X-axis 0

        PORTH &= ~(_BV(PH6));
        x_calibration[0] |= _BV(6);
        x_calibration[0] &= ~(_BV(7));
        spi_write_master(x_calibration,2);
        PORTH |= _BV(PH6);

        // Y-axis 0 

        PORTH &= ~(_BV(PH6));
        y_calibration[0] |= _BV(6);
        y_calibration[0] &= ~(_BV(7));
        spi_write_master(y_calibration,2);
        PORTH |= _BV(PH6);

        // Z-axis -3
        
        PORTH &= ~(_BV(PH6));
        z_calibration[0] |= _BV(6);
        z_calibration[0] &= ~(_BV(7));
        spi_write_master(z_calibration,2);
        PORTH |= _BV(PH6);

    }else{
        init_i2c();
        i2c_master_write(ADXL345, dataInit, 2, 1);
        i2c_master_write(ADXL345, x_calibration, 2, 1);
        i2c_master_write(ADXL345, y_calibration, 2, 1);
        i2c_master_write(ADXL345, z_calibration, 2, 1);
    }
}

void get_axis_readings(int16_t* data, uint8_t spi){
    uint8_t data_register[] = {0x32};
    uint8_t retrieved_data[] = {0,0,0,0,0,0};

    if(spi){
        PORTH &= ~(_BV(PH6));
        // specify address to read later
        // dont deselect the slave
        // Set bit 7 to indicate read and MB
        data_register[0] |= _BV(6) | _BV(7);
        spi_write_master(data_register,1);

        // read the 6 bytes that are returned
        spi_read_master(retrieved_data,6);
        PORTH |= _BV(PH6);
    }else{
        i2c_master_write(ADXL345, data_register, 1, 0);
        i2c_master_read(ADXL345, retrieved_data, 6);
    }
    
    
    int16_t X_out, Y_out, Z_out;

    X_out = (int16_t)retrieved_data[0] | ((int16_t)retrieved_data[1] << 8);
    // X_out = X_out/256;
    Y_out = (int16_t)retrieved_data[2] | ((int16_t)retrieved_data[3] << 8);
    // Y_out = Y_out/256;
    Z_out = (int16_t)retrieved_data[4] | ((int16_t)retrieved_data[5] << 8);
    // Z_out = Z_out/256;

    data[0] = X_out;
    data[1] = Y_out;
    data[2] = Z_out;
}