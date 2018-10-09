#include <SPI.h>
#include <Wire.h>

#define MSB   0b1111111100000000 //8 MS bits 
#define MS3B  0b0000011100000000 //3 MS bits 
#define LSB   0b0000000011111111 //8 LS bits

/*
 * Sensor de temperatura 
 */
int sensorTemperatura = A0;
float temperatura;
byte x = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin(); // join i2c bus (address optional for master)
  temperatura = 0;

  medeTemperatura();
  Serial.println(temperatura);
  //Serial.println(temperatura);
  writeEeprom(0, temperatura);
  //Serial.println(readEeprom(0));
}


//void loop() {
////  Wire.beginTransmission(8); // transmit to device #8
////  Wire.write("x is ");        // sends five bytes
////  Wire.write(x);              // sends one byte
////  Wire.endTransmission();    // stop transmitting
////
////  x++;
////  delay(500);
//}

void loop() {
  
}

void medeTemperatura(){
  temperatura = (analogRead(sensorTemperatura)/1023.0)*5*100;
}

void writeEeprom(unsigned int memoryPosition, unsigned int temperatura){
  
  /*
   * Escrita do MS byte da temperatura
   */
  //byte pageNumber = (byte)((memoryPosition & MS3B)>>8); // 3 bits of page adress
  //byte i2cAddress = 0b01010000 | (byte)pageNumber; //set the I2C addresss
  //Wire.beginTransmission(i2cAddress);

  //byte memAddress = (byte)(memoryPosition & LSB); //completando endereço de 11
  //Wire.write(memAddress);

  //byte msbTemp = (byte)((temperatura & MSB)>>8);  // tiramos os ultimos bits, e jogamos os mais significativos pra direita
  //Wire.write(msbTemp);
  byte dado = 0xF0;
  int add = 1010000 | (memoryPosition >> 8) & 0x7;
  Wire.write(add & 0xFF);
  Wire.write(dado);

  Serial.print("End transmission 1: = ");
  int c = Wire.endTransmission();
  Serial.println(c);
  
  //Wire.endTransmission();

  delay(5);

  /*
   * Escrita do LS byte da temperatura
   */
  //Wire.beginTransmission(i2cAddress);
  //o MS byte da temperatura é salvo no endereço 0bxx...xx0 
  //o LS byte da temperatura é salvo no endereço 0bxx...xx1
  //assim podemos incrementar memAddress sem problemas
  //memAddress += 1;
  //Wire.write(memAddress);
  
  //byte lsbTemp = (byte)(temperatura & LSB); // tem que mascarar com 0xFF
  //Wire.write(lsbTemp);

  //Serial.print("End transmission 2: = ");
  //c = Wire.endTransmission();
  //Serial.println(c);
  
  //delay(5);
}

byte readEeprom(unsigned int memPos) {
  byte valor = 0x00;
  byte pageNumber = (byte)((memPos & MS3B)>>8); // 3 bits of page adress
  byte i2cAddress = 0b01010000 | (byte)pageNumber; //set the I2C addresss
  Wire.beginTransmission(i2cAddress);
  
  byte memAddress = (byte)(memPos & LSB); //completando endereço de 11
  Wire.write(memAddress);
  memAddress += 1;
  Wire.write(memAddress);
  
  Wire.requestFrom(i2cAddress, 1);
  if (Wire.available()) {
    Serial.println("Wire ta available");
    valor = Wire.read();
  }  
  
  Wire.endTransmission();
  
  return valor;
}

