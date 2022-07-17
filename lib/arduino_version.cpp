#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#define ADXL345 0x53

float X_out, Y_out, Z_out; 
void setup()
{
    // initialize both serial ports:
    Serial.begin(9600);
    // Serial2.begin(115200);
    Wire.begin();
    Wire.beginTransmission(ADXL345);
    Wire.write(0x2D);
    Wire.write(8);
    Wire.endTransmission();   
    delay(10);
}

void loop()
{

    // // read from port 1, send to port 0:
    // if (Serial2.available())
    // {
    //     int inByte = Serial2.read();
    //     Serial.write(inByte);
    // }

    // // read from port 0, send to port 1:
    // if (Serial.available())
    // {
    //     int inByte = Serial.read();
    //     Serial2.write(inByte);
    // }
    Wire.beginTransmission(ADXL345);
    Wire.write(0x32);
    Wire.endTransmission(false);
    Wire.requestFrom(ADXL345, 6, true);
    X_out = Wire.read() | (Wire.read() << 8);
    X_out = X_out/256;
    Y_out = Wire.read() | (Wire.read() << 8);
    Y_out = Y_out/256;
    Z_out = Wire.read() | (Wire.read() << 8);
    Z_out = Z_out/256;

    Serial.print("X= ");
    Serial.print(X_out);
   
    Serial.print("Y= ");
    Serial.print(Y_out);
    
    Serial.print("Z= ");
    Serial.println(Z_out);
    // delay(1000);
}