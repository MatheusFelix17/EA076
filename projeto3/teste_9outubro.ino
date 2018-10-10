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
  unsigned int temperaturaEscrita = (unsigned int)(temperatura * 10);
  
  Serial.println(temperaturaEscrita);
//  writeEeprom(0, temperaturaEscrita);
  Serial.print("Retorno da READ: ");
  Serial.println(readEeprom(10));
}

void loop() {
  
}

void medeTemperatura(){
  temperatura = (analogRead(sensorTemperatura)/1023.0)*5*100;
}

void writeEeprom(unsigned int memoryPosition, unsigned int temperatura){
  
  /*
   * Escrita do MS byte da temperatura
   */
  byte pageNumber = (byte)((memoryPosition & MS3B)>>8); // 3 bits of page adress
  //Serial.println(pageNumber, BIN);
  byte i2cAddress = 0b01010000 | (byte)pageNumber; //set the I2C addresss
  //Serial.println(i2cAddress,BIN);
  Wire.beginTransmission(i2cAddress);

  byte memAddress = (byte)(memoryPosition & LSB); //completando endereço de 11
  //Serial.println(memAddress,BIN);
  Wire.write(memAddress);

  byte msbTemp = (byte)((temperatura & MSB)>>8);  // tiramos os ultimos bits, e jogamos os mais significativos pra direita
  //Serial.print("MSB para escrita: ");
  //Serial.println(msbTemp,BIN);
  Wire.write(msbTemp);
 

  //Serial.print("End transmission 1: = ");
  //int c = Wire.endTransmission();
  //Serial.println(c);
  Wire.endTransmission();

  delay(5);

  /*
   * Escrita do LS byte da temperatura
   */
  Wire.beginTransmission(i2cAddress);
  //o MS byte da temperatura é salvo no endereço 0bxx...xx0 
  //o LS byte da temperatura é salvo no endereço 0bxx...xx1
  //assim podemos incrementar memAddress sem problemas
  memAddress += 1;
  Wire.write(memAddress);
  
  byte lsbTemp = (byte)(temperatura & LSB); // tem que mascarar com 0xFF
  //Serial.print("LSB para escrita: ");
  //Serial.println(lsbTemp,BIN);
  Wire.write(lsbTemp);

  //Serial.print("End transmission 2: = ");
//  c = Wire.endTransmission();
//  Serial.println(c);

  Wire.endTransmission();
  delay(5);
}

unsigned int readEeprom(unsigned int memPos) {
  unsigned int valor = 0;
  unsigned int valorLSB = 0;
  unsigned int valorMSB = 0;
  
  byte pageNumber = (byte)((memPos & MS3B)>>8); // 3 bits of page adress
  byte i2cAddress = 0b01010000 | (byte)pageNumber; //set the I2C addresss
  Wire.beginTransmission(i2cAddress);
  
  byte memAddress = (byte)(memPos & LSB); //completando endereço de 11
  Wire.write(memAddress);
  Wire.endTransmission();
  Wire.requestFrom(i2cAddress, 2);
  if (Wire.available()) {
    valorMSB = Wire.read();
    valorLSB = Wire.read();
  }  

//  Serial.print("valor MSB: ");
//  Serial.println(valorMSB,BIN);
//  Serial.print("valor LSB: ");
//  Serial.println(valorLSB,BIN);

  valor = valor | (valorMSB <<8);
  valor = valor | valorLSB;

//  Serial.print("valor na memoria: ");
//  Serial.println(valor);
  
  return valor;
}

