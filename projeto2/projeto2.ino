#include "TimerOne.h"
/*Bibliotecas para a interface com o monitor LCD*/
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>


/*
  Portas utilizadas:
  MOTOR:
*/
int enable = 7;
int pinoA = 6;
int pinoB = 5;
int velocidade = 0;
int vel = 0;
/*UART
*/
int tx = 1;
int rx = 0;

/*ENCODER
*/
int encoder_interrupt = 3;

/*
 * Instancia a porta
 * 
 * pin 8 - Serial clock out (SCLK)
 * pin 9 - Serial data out (DIN)
 * pin 10 - Data/Command select (D/C)
 * pin 11 - LCD chip select (CS/CE)
 * pin 12 - LCD reset (RST)
 */

Adafruit_PCD8544 display = Adafruit_PCD8544(8, 9, 10, 11, 12);

/*
 * FLAG VENTILADOR
*/
int flag_vent = 0;

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
  Timer1.pwm(enable, velocidade);                // setup pwm on pin 9, 50% duty cycle\

  /*
   * Inicializa o Visor LCD
  */
  display.begin();
  display.setContrast(40); //Ajusta o contraste do display
  display.clearDisplay();   //Apaga o buffer e o display
  display.setTextSize(1);  //Seta o tamanho do texto
  display.setTextColor(BLACK); //Seta a cor do texto
  display.setCursor(0,0);  //Seta a posição do cursor
  //display.println("Arduino & Cia!");
  display.display();
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
//      Serial.print("tamanho do comando válido, que é: ");
//      Serial.println(content);

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
//            Serial.print("Substring: ");
//            Serial.println(resto);
            if(isDigit(resto[0]) && isDigit(resto[1]) && isDigit(resto[2])) {
//              Serial.println("Os 3 valores são numeros");
              vel = resto.toInt();  
//              Serial.print("Numero convertido = ");
//              Serial.println(vel);   
              if (vel >= 0 && vel <= 100) {
//                Serial.println("Comando da velocidade OK!");    
                String resposta = "OK VEL ";
                //resposta.concat(vel);
                //resposta.concat('%');   
                Serial.println(resposta);
                //CHAMA A FUNÇÃO
                seta_speed(vel);
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
   flag_vent = 1;
   digitalWrite(pinoA, HIGH);
   digitalWrite(pinoB, LOW);
   print_ventilador();
   //digitalWrite(enable, HIGH);
}

void exaustor()
{
   parado();
   flag_vent = 0;
   digitalWrite(pinoA, LOW);
   digitalWrite(pinoB, HIGH);
   print_exaustor();
   //digitalWrite(enable, HIGH);
}

void parado()
{
   //digitalWrite(pino, !digitalRead(pino));
   seta_speed(0);
   print_parado();
   delay(200);
}

void seta_speed(int vel)
{
   velocidade = (int) (vel*1024/100);
   Timer1.pwm(enable, velocidade);

  if (flag_vent == 1){
    print_ventilador();
  }
  else if (flag_vent == 0){
    print_exaustor();
  }
}



void print_ventilador(){
  String string_velocidade = "";
  display.clearDisplay();   //Apaga o buffer e o display
  display.setCursor(0,0);
  display.println("VENTILADOR");
  string_velocidade += vel;
  string_velocidade += "%";
  display.println(string_velocidade);
  display.display();
}

void print_exaustor(){
  String string_velocidade = "";
  display.clearDisplay();   //Apaga o buffer e o display
  display.setCursor(0,0);
  display.println("EXAUSTOR");
  string_velocidade += vel;
  string_velocidade += "%";
  display.println(string_velocidade);
  display.display();
}


void print_parado(){
  String string_velocidade = "";
  display.clearDisplay();   //Apaga o buffer e o display
  display.setCursor(0,0);
  display.println("PARADO");
  string_velocidade += vel;
  string_velocidade += "%";
  display.println(string_velocidade);
  display.display();
}

