
#include "TimerOne.h"
#include <SPI.h>
/*Bibliotecas para a interface com o monitor LCD*/
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Wire.h>


/*
 * VISOR LCD
 * 
 * pin 13 (SPI) - Serial clock out (SCLK)
 * pin 11 (SPI)- Serial data out (DIN)
 * pin 8 - Data/Command select (D/C)
 * pin 9 - LCD chip select (CS/CE)
 * pin 2 - LCD reset (RST)
 */
Adafruit_PCD8544 display = Adafruit_PCD8544(8, 9, 2);

/*
 * Contadores de tempo
 */

unsigned int tempoInterrupt;

/*
 * Sensor de temperatura 
 */
int sensorTemperatura = A0;
float temperatura;

/*
 * Teclado Matricial
 */
int c1 = A3;
int c2 = A2;
int c3 = A1;
int l1 = 7;
int l2 = 6;
int l3 = 5;
int l4 = 4;

void setup()
{
  /*
   * Inicializa o serial para PC
   */
  Serial.begin(9600);

  /*
   * Inicializa o timer e a interrupção por tempo
   */
  Timer1.initialize(1000);         //inicializa o timer com intervalo de tempo de 1ms
  Timer1.attachInterrupt(somaTempo);

  /*
   * Inicializa o Visor LCD
   */
  display.begin();
  display.setContrast(40); //Ajusta o contraste do display
  display.clearDisplay();   //Apaga o buffer e o display
  display.setTextSize(1);  //Seta o tamanho do texto
  display.setTextColor(BLACK); //Seta a cor do texto
  display.setCursor(0,0);  //Seta a posição do cursor
  display.println("Arduino & Cia!");
  display.display();

  /*
   * inicializa os contadores de tempo
   */
  tempoInterrupt = 0;
  /*
   * inicializa o valor de temperatura
   */
  temperatura = 0;
  /*
   * inicializa teclado matricial 
   */
  pinMode(c1, INPUT_PULLUP);
  pinMode(c2, INPUT_PULLUP);
  pinMode(c3, INPUT_PULLUP);
  pinMode(l1, OUTPUT);
  pinMode(l2, OUTPUT);
  pinMode(l3, OUTPUT);
  pinMode(l4, OUTPUT);
  digitalWrite(l1, HIGH);
  digitalWrite(l2, HIGH);
  digitalWrite(l3, HIGH);
  digitalWrite(l4, HIGH);
  /*
   * Inicializa o I2C para a EEPROM
   */
  Wire.begin();
}



void loop(){
  /*
   * Le a entrada serial enviada via Bluetooth
   */
  medeTemperatura();
  Serial.println(temperatura);


}

/*
 * Incrementa o contador de tempo
 */
void somaTempo(){
  tempoInterrupt += 1;
}


void medeTemperatura(){
  temperatura = (analogRead(sensorTemperatura)/1023.0)*5*100;
}

void leTeclado(){
  short i = 0;
  for (i = 0; i < 4; i++){
    switch(i){
      case 0:
        digitalWrite(l4, HIGH);
        digitalWrite(l1, LOW); 
        break;
      case 1:
        digitalWrite(l1, HIGH);
        digitalWrite(l2, LOW);
        break;
      case 2:
        digitalWrite(l2, HIGH);
        digitalWrite(l3, LOW);
        break;
      case 3:
        digitalWrite(l3, HIGH);
        digitalWrite(l4, LOW);
        break;
      default:
        break;
    }
  }
}

