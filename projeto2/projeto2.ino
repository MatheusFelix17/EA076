#include "TimerOne.h"
#include <SPI.h>
/*Bibliotecas para a interface com o monitor LCD*/
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>


/*
  Portas utilizadas:
  MOTOR:
*/
char enable = 9;
int pinoA = 6;
int pinoB = 7;
int vel = 0;
int RPM = 0;
int velocidade_RPM;
int contador_interrupt;
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
 * pin 13 (SPI) - Serial clock out (SCLK)
 * pin 11 (SPI)- Serial data out (DIN)
 * pin 9 - Data/Command select (D/C)
 * pin 8 - LCD chip select (CS/CE)
 * pin 4 - LCD reset (RST)
 */

Adafruit_PCD8544 display = Adafruit_PCD8544(5, 8, 4);

/*
 * FLAG VENTILADOR
*/
int flag_vent = 0;

/*
 * Contadores de tempo
 */

unsigned int tempo_interrupt;

void setup()
{
  /*
   * Inicializa o bluetooth
   */
  Serial.begin(9600);

  /*
   * Inicializa o timer e a interrupção por tempo
   */
  Timer1.initialize(1000);         //inicializa o timer com intervalo de tempo de 1ms
  Timer1.attachInterrupt(somaTempo);
  /*
   * Inicializa o motor
   */
  Timer1.pwm(enable, vel);                
  /**
   * Inicializa a função de tratamento da interrupçào externa (encoder)
   */
   attachInterrupt(digitalPinToInterrupt(3), somaRPM, RISING);
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

  /*
   * inicializa os contadores de tempo
   */
   tempo_interrupt = 0;
   /*
    * Inicializa o contador de passagens da haste na frente do sensor (calculo de velocidade)
    */
    contador_interrupt = 0;
    velocidade_RPM = 0;
}



void loop(){
  /*
   * Le a entrada serial enviada via Bluetooth
   */
  trata_entrada();
  calcula_velocidade();
}

/*
 * Incrementa o contador de tempo
 */
void somaTempo(){
  tempo_interrupt += 1;
}

/*
 * Incrementa o contador de interrupções externas (encoder)
 */
void somaRPM(){
  contador_interrupt += 1;
}


/*
 * Trata A entrada serial que chega pelo HC-06 (módulo Bluetooth)
 */
void trata_entrada(){
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
      /**
       * Checa se o que foi recebido é algum comando valido e executa o comando
       */
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
        String string_vel_retorno = "";
        string_vel_retorno += "VEL: ";
        string_vel_retorno +=velocidade_RPM;
        string_vel_retorno += " RPM";
        Serial.println(string_vel_retorno);
      }

      else if (content.substring(0,4) == "VEL " || content.substring(0,4) == "VEL*" ) {
        
        //vamos verificar se os próximos digitos de VEL
        String resto = content.substring(4,8);
        if (resto == "") {
          Serial.println("ERRO: PARÂMETRO AUSENTE");
        }

        else {
          if (resto[3] == '*') {
            if(isDigit(resto[0]) && isDigit(resto[1]) && isDigit(resto[2])) {
              vel = resto.toInt();    
              if (vel >= 0 && vel <= 100) {  
                String resposta = "OK VEL "; 
                Serial.println(resposta);
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

/*
 * Calcula a velocidade em RPM
 */
void calcula_velocidade(){
   if (tempo_interrupt >= 100){ //se passou 100ms
      float velocidade_hz = contador_interrupt * 10.0 / 2;
      velocidade_RPM = 60*velocidade_hz;
      //chama a função para escrever na tela
      seta_speed(vel);
      contador_interrupt = 0;
      tempo_interrupt = 0;
   }
}

/*
 * Funcao para ativação do motor no modo ventilador
 */
void ventilador()
{
   flag_vent = 1;
   parado();
   digitalWrite(pinoA, HIGH);
   digitalWrite(pinoB, LOW);
   print_ventilador();
}

/*
 * Funcao para ativação do motor no modo exaustor
 */
void exaustor()
{
   flag_vent = 0;
   parado();
   digitalWrite(pinoA, LOW);
   digitalWrite(pinoB, HIGH);
   print_exaustor();
}

/*
 * Função para parar o motor
 */
void parado()
{
   digitalWrite(pinoA, LOW);
   digitalWrite(pinoB, LOW);
   print_parado();
   delay(200);
}

/*
 * Função que define a velocidade do motor baseado no duty cicle do PWM que ativa ele (enable)
 * O parametro de entrada da função varia de 0 a 100, porém o parametro do PWM é de 0 a 1023, 
 * então fazemos a transformação dos valores
 */
void seta_speed(int vel)
{
  //Serial.println(vel);
   int velocidade = (int)(vel*(1024.0/100));
   Timer1.setPwmDuty(enable, velocidade);

  if (flag_vent == 1){
    print_ventilador();
  }
  else if (flag_vent == 0){
    print_exaustor();
  }
}


/**
 * Funcao para imprimir display LCD o modo ventilador
 */
void print_ventilador(){
  String string_velocidade = "";
  String string_rpm = "";
  display.clearDisplay();   //Apaga o buffer e o display
  display.setCursor(0,0);
  display.println("VENTILADOR");
  string_velocidade += vel;
  string_velocidade += "%";
  display.println(string_velocidade);
  string_rpm += velocidade_RPM;
  string_rpm += " RPM";
  display.println(string_rpm);
  display.display();
}

/**
 * Funcao para imprimir display LCD o modo exaustor
 */
void print_exaustor(){
  String string_velocidade = "";
  String string_rpm = "";
  display.clearDisplay();   //Apaga o buffer e o display
  display.setCursor(0,0);
  display.println("EXAUSTOR");
  string_velocidade += vel;
  string_velocidade += "%";
  display.println(string_velocidade);
  string_rpm += velocidade_RPM;
  string_rpm += " RPM";
  display.println(string_rpm);
  display.display();
}

/**
 * Funcao para imprimir display LCD quando o motor está parado
 */
void print_parado(){
  String string_velocidade = "";
  String string_rpm = "";
  display.clearDisplay();   //Apaga o buffer e o display
  display.setCursor(0,0);
  display.println("PARADO");
  string_velocidade += vel;
  string_velocidade += "%";
  display.println(string_velocidade);
  string_rpm += velocidade_RPM;
  string_rpm += " RPM";
  display.println(string_rpm);
  display.display();
}

