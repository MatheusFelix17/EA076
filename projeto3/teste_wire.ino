#include <Wire.h>

#define MSB   B1111111100000000 //8 MS bits 
#define MS3B  B0000011100000000 //3 MS bits 
#define LSB   B0000000011111111 //8 LS bits

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
}

byte x = 0;

//void loop() {
////  Wire.beginTransmission(8); // transmit to device #8
////  Wire.write("x is ");        // sends five bytes
////  Wire.write(x);              // sends one byte
////  Wire.endTransmission();    // stop transmitting
////
////  x++;
////  delay(500);
//}


void writeEeprom(unsigned int memoryPosition, unsigned int temperatura){

  /*
   * Escrita do MS byte da temperatura
   */
  byte pageNumber = (byte)((memoryPosition & MS3B)>>8); // 3 bits of page adress
  byte i2cAddress = B01010000 | (byte)pageNumber; //set the I2C addresss
  Wire.beginTransmission(i2cAddress);

  byte memAddress = (byte)(memoryPosition & LSB);
  Wire.write(memAddress);

  byte msbTemp = (byte)((temperatura & MSB)>>8);
  Wire.write(msbTemp);

  Wire.endTransmission();

  delay(5000);

  /*
   * Escrita do LS byte da temperatura
   */
  Wire.beginTransmission(i2cAddress);
  //o MS byte da temperatura é salvo no endereço 0bxx...xx0 
  //o LS byte da temperatura é salvo no endereço 0bxx...xx1
  //assim podemos incrementar memAddress sem problemas
  memAddress += 1;
  Wire.write(memAddress);
  
  byte lsbTemp = (byte)(temperatura & LSB);
  Wire.write(lsbTemp)

  Wire.endTransmission();
  
  delay(5000);
}

