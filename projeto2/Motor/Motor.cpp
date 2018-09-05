#include <Motor.h>

Motor::Motor(int pinoA, int pinoB, int enable)
{
   pinMode(pinoA, OUTPUT);
   pinMode(pinoB, OUTPUT);
   pinMode(enable, OUTPUT);
}

void Motor::ventilador()
{
   digitalWrite(pinoA, HIGH);
   digitalWrite(pinoB, LOW);
   digitalWrite(enable, HIGH);
}

void Motor::exaustor()
{
   digitalWrite(pinoA, LOW);
   digitalWrite(pinoB, HIGH);
   digitalWrite(enable, HIGH);
}

void Motor::parado()
{
   //digitalWrite(pino, !digitalRead(pino));
   digitalWrite(enable, LOW);
}

void Motor::seta_speed(int vel)
{
    //speed do motor = velocidade, ainda nao sei como atribuir isso
}
