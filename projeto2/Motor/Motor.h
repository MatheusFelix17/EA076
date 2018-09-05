#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor
{
public:
  Motor(int pinoA, int pinoB, int enable);
  void ventilador();
  void exaustor();
  void parado();
  void seta_speed(int vel);

};


#endif
