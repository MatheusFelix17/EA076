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
  Serial.begin(9600);
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
  char c;
  String content = "";
  int limite = 8; //tamanho maximo de um comando permitido
  int contador = 0;
  // Se receber algo pela serial
    if (Serial.available() > 0){
    // Lê toda string recebida
    //String recebido = leStringSerial();
    while(Serial.available() > 0) {
      c = Serial.read();
      contador++;
      if (c != '\n') {
        content.concat(c);
      }
      delay(10);
    }  

    if (contador > 8) {
      Serial.println("Erro: tamanho maximo do comando excedido!");  //ERRO: COMANDO INEXISTENTE
    }
    else {
      Serial.print("tamanho do comando válido, que é: ");
      Serial.println(content);

      if (content == "VENT*") {
        Serial.println("OK VENT");
        ventilador();
      }

      else if (content == "EXAUST*") {
        Serial.println("OK EXAUST");
        exaustor();
      }
    
      else if (content == "PARA*") {
        Serial.println("OK PARA");
        parado();
      }

      else if (content == "RETVEL*") {
        Serial.print("VEL");
        Serial.print(" X ");  //numero
        Serial.println("RPM");

        //chama alguma coisa aqui
      }

      else if (content.substring(0,3) == "VEL") {
        
        //vamos verificar se os próximos digitos de VEL
        String resto = content.substring(4,8);
        if (resto == "") {
          Serial.println("ERRO: PARÂMETRO AUSENTE");
        }

        else {
          if (resto[3] == '*') {
            Serial.print("Substring: ");
            Serial.println(resto);
            if(isDigit(resto[0]) && isDigit(resto[1]) && isDigit(resto[2])) {
              Serial.println("Os 3 valores são numeros");
              int num = resto.toInt();  
              Serial.print("Numero convertido = ");
              Serial.println(num);   
              if (num >= 0 && num <= 100) {
                Serial.println("Comando da velocidade OK!");    
                String resposta = "OK VEL ";
                resposta.concat(num);
                resposta.concat('%');   
                Serial.println(resposta);
                //CHAMA A FUNÇÃO
                seta_speed(num);
              }
              else
                Serial.println("ERRO: PARÂMETRO INCORRETO");        
            }
            else
              Serial.println("ERRO: PARÂMETRO INCORRETO");
          }
          else 
            Serial.println("ERRO: COMANDO INCORRETO");  //OU PARAMETRO
        }
        
      }

      else 
        Serial.println("ERRO: COMANDO INEXISTENTE");
      
    }
  
  }

}


void ventilador()
{
   parado();
   digitalWrite(pinoA, HIGH);
   digitalWrite(pinoB, LOW);
   //digitalWrite(enable, HIGH);
}

void exaustor()
{
   parado();
   digitalWrite(pinoA, LOW);
   digitalWrite(pinoB, HIGH);
   //digitalWrite(enable, HIGH);
}

void parado()
{
   //digitalWrite(pino, !digitalRead(pino));
   seta_speed(0);
   delay(200);
}

void seta_speed(int vel)
{
   velocidade = (int) (vel*1024/100);
   Timer1.pwm(enable, velocidade);

}

