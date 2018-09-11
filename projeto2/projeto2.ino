

/*
    Main code

*/

#include "TimerOne.h"


/*
  Portas utilizadas:
  MOTOR:
*/
int enable = 7;
int pinoA = 6;
int pinoB = 5;
int velocidade = 512;
/*UART
*/
int tx = 1;
int rx = 0;

/*ENCODER
*/
int encoder_interrupt = 3;

/*SPI
*/
int SPI = 12;


void setup()
{
  pinMode(10, OUTPUT);
  Timer1.initialize(500000);         // initialize timer1, and set a 1/2 second period
  /*
   * Inicializa o motor
   */
   pinMode(pinoA, OUTPUT);
   pinMode(pinoB, OUTPUT);
   Timer1.pwm(enable, velocidade);                // setup pwm on pin 9, 50% duty cycle
}



void loop()
{
  ventilador();
}


void ventilador()
{
   digitalWrite(pinoA, HIGH);
   digitalWrite(pinoB, LOW);
   //digitalWrite(enable, HIGH);
}

void exaustor()
{
   digitalWrite(pinoA, LOW);
   digitalWrite(pinoB, HIGH);
   //digitalWrite(enable, HIGH);
}

void parado()
{
   //digitalWrite(pino, !digitalRead(pino));
   seta_speed(0);
}

void seta_speed(int vel)
{
   velocidade = (int) (vel*1024/100);
   Timer1.pwm(enable, velocidade);

}


