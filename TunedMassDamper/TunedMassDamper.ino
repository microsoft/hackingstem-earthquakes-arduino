
//===----------------__ Hacking STEM Seismograph Arduino __------------===//
// For use with the Using Computational Thinking to Understand Earthquakes 
// Tuned Mass Damper lesson plan available from Microsoft Education 
// Workshop at http://aka.ms/hackingSTEM
//
// Overview:
// This code captures accelerometer data over i2c, but not relying on any 
// external libraries, and sends it over serial.
//
// This project uses an Arduino UNO microcontroller board, information at:
// https://www.arduino.cc/en/main/arduinoBoardUno
//
// This project uses the LIS3DH accelerometer, which is available in a
// prototype friendly fashion at https://www.adafruit.com/product/2809
//
// Please note, we have avoided requiring a 3rd party library that would 
// normally be required utilize the LIS3DH, however much of this project
// included some potentially confusing fairly low level code used to
// interface with the LIS3DH.  
//
// Comments, contributions, suggestions, bug reports, and feature requests
// are welcome! For source code and bug reports see:
// http://github.com/[TODO github path to Hacking STEM]
//
// Copyright 2017 Adi Azulay Microsoft EDU Workshop - HackingSTEM
// MIT License terms detailed in LICENSE.txt 
//===----------------------------------------------------------------------===//

#include <Wire.h>


// List of registers used by accelerometer
#define LIS3DH_ADDRESS           0x18
#define LIS3DH_REG_STATUS1       0x07
#define LIS3DH_REG_WHOAMI        0x0F
#define LIS3DH_REG_TEMPCFG       0x1F
#define LIS3DH_REG_CTRL1         0x20
#define LIS3DH_REG_CTRL3         0x22
#define LIS3DH_REG_CTRL4         0x23
#define LIS3DH_REG_OUT_Y_L       0x2A
#define LIS3DH_REG_OUT_Y_H       0x2B

#define LIS3DH_8G_SCALE_FACTOR  .00024414f

#define LIS3DH_RANGE_8_G         0b10   // +/- 8g

const int dataRate = 10; // Frequency in milliseconds that data is sent to Excel

void setup(){
  // Initialize communication with LIS3DH
  Wire.begin();
  Wire.beginTransmission(LIS3DH_ADDRESS);  //Connects to LIS3DH via i2c
  Wire.write (LIS3DH_REG_WHOAMI);          //Check that LIS3DH is connected
  Wire.endTransmission(true);
  Wire.requestFrom (LIS3DH_ADDRESS, 1);
  uint8_t deviceID = Wire.read();

  // hang device if LIS3DH isn't connected
  while (deviceID != 0x33){                 
    delay(1);
  }

  //Turn on all axes and set to normal mode
  writeRegister8 (LIS3DH_REG_CTRL1, 0x07); 

  //Set data rate to 400 mHz, used to manage power consuption
  //set data rate
  uint8_t accelDataRate = readRegister8 (LIS3DH_REG_CTRL1);
  accelDataRate &= ~(0xF0);
   //Change variable to write
  accelDataRate |= 0b0111 << 4;               
  writeRegister8 (LIS3DH_REG_CTRL1, accelDataRate);      

  //Enables High Resolution and BDU
  writeRegister8 (LIS3DH_REG_CTRL4, 0x88);    

  // DRDY on INT1
  writeRegister8 (LIS3DH_REG_CTRL3, 0x10);       

  //Activate ADC outputs
  writeRegister8 (LIS3DH_REG_TEMPCFG, 0x80);      

  //Set read scale
  uint8_t rangeControl = readRegister8 (LIS3DH_REG_CTRL4);
  rangeControl &= ~(0x30);
  //Change variable to write make sure to also update the scale factor
  rangeControl |= LIS3DH_RANGE_8_G << 4;                
  writeRegister8 (LIS3DH_REG_CTRL4, rangeControl);

  Serial.begin (9600);
}

void loop(){
    // Communicate with LIS3DH
    Wire.beginTransmission(LIS3DH_ADDRESS);
    Wire.write(LIS3DH_REG_OUT_Y_L | 0x80);
    Wire.endTransmission();

    Wire.requestFrom(LIS3DH_ADDRESS, 2);

    // wait for next two bytes
    while (Wire.available() < 2);

    uint8_t yla = Wire.read();
    uint8_t yha = Wire.read();

    // get yAxis bits out of return message
    float yAxis = yha << 8 | yla;

    // scale value for 8Gs
    yAxis = yAxis * LIS3DH_8G_SCALE_FACTOR;

    // send data to excel via serial    
    sendSerialData(yAxis);
}

void sendSerialData(float dataValue){
  Serial.print (dataValue);
  Serial.println ();

  delay (dataRate);
}

//===----------------------------------------------------------------------===//
// Code below is for interfacing with LIS3DH
//===----------------------------------------------------------------------===//

void writeRegister8 (uint8_t reg, uint8_t val){
  Wire.beginTransmission (LIS3DH_ADDRESS);
  Wire.write (reg);
  Wire.write (val);
  Wire.endTransmission();
}

uint8_t readRegister8 (uint8_t reg){
  Wire.beginTransmission (LIS3DH_ADDRESS);
  Wire.write (reg);
  Wire.endTransmission();

  Wire.requestFrom(LIS3DH_ADDRESS, 1);
  uint8_t val = Wire.read();
  return val;
  Wire.endTransmission();

}
