/*
Tuned Mass Dampner (TMD) code using Arduino UNO and Adafruit LIS3DH breakout board
More detials on TMD: https://www.microsoft.com/en-us/education/education-workshop/seismograph.aspx
More details on LIS3DH: https://www.adafruit.com/product/2809

This code captures accelerometer data over i2c, but not relying on any external libraries, and sends it over serial.


Adi Azulay, 2017 Microsoft EDU Workshop
*/

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

const int dataRate = 10;      // Change this variable to increase the frequency that data is sent to Excel

void setup(){
                                            //Initilize LIS3DH and set to +/- 16g Scale Factor
  Wire.begin();

  Wire.beginTransmission(LIS3DH_ADDRESS);   //Connects to LIS3DH via i2c
  Wire.write (LIS3DH_REG_WHOAMI);            //Check that board is connected
  Wire.endTransmission(true);
  Wire.requestFrom (LIS3DH_ADDRESS, 1);
  uint8_t deviceID = Wire.read();

  while (deviceID != 0x33){
    delay(1);
  }

  writeRegister8 (LIS3DH_REG_CTRL1, 0x07);      //Turn on all axes and set to normal mode

  //set data rate
  uint8_t accelDataRate = readRegister8 (LIS3DH_REG_CTRL1);
  accelDataRate &= ~(0xF0);
  accelDataRate |= 0b0111 << 4;                //Change variable to write
  writeRegister8 (LIS3DH_REG_CTRL1, accelDataRate);      //Set data rate to 400 mHz, used to manage power consuption


  writeRegister8 (LIS3DH_REG_CTRL4, 0x88);      //Enebles High Res and BDU

  writeRegister8 (LIS3DH_REG_CTRL3, 0x10);       // DRDY on INT1

  writeRegister8 (LIS3DH_REG_TEMPCFG, 0x80);      //Activate ADC outputs

  //Set read scale
  uint8_t rangeControl = readRegister8 (LIS3DH_REG_CTRL4);
  rangeControl &= ~(0x30);
  rangeControl |= LIS3DH_RANGE_8_G << 4;                //Change variable to write make sure to also update the scale factor
  writeRegister8 (LIS3DH_REG_CTRL4, rangeControl);


  Serial.begin (9600);
}

void loop(){
  float total = 0;

//  for (int i = 0; i <= 100; i++){
    Wire.beginTransmission(LIS3DH_ADDRESS);
    Wire.write(LIS3DH_REG_OUT_Y_L | 0x80);
    Wire.endTransmission();

    Wire.requestFrom(LIS3DH_ADDRESS, 2);
    while (Wire.available() < 2);

    uint8_t yla = Wire.read();
    uint8_t yha = Wire.read();


    float yAxis = yha << 8 | yla;

    yAxis = yAxis * LIS3DH_8G_SCALE_FACTOR;

  //  total = total + yAxis;

//  }

  float average = total / 100;
  sendSerialData(yAxis);
}

void sendSerialData(float dataValue){
  Serial.print (dataValue);
  Serial.println ();

  delay (dataRate);
}

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
