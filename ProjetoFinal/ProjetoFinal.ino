/*Bibliotecas para a interface com o monitor LCD*/
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <TimerOne.h>
#include <Wire.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(9, 8, 7); //

/*
 * Teclado Matricial
 */
int C1 = 6;
int C2 = 5;
int C3 = 4;
int L1 = A0;
int L2 = A1;
int L3 = A2;
int L4 = A3;

/**
 * Variavel de estado para maquina de estados do loop principal
 */
int estado;

// flags para detecção do botão:
short solto = 0, pressionado = 0;

/**
 * Contadores de tempo
 */
unsigned short momento_pressionado;
unsigned short tempo_display;

/*flag para leitura de teclado*/
bool pressionou = false;
bool soltou = true;
int lendo_tecla = 0;
int digito_1, digito_2;
int numero;

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);

  /*
   * Inicializa o Visor LCD
  */
  display.begin();
  display.setContrast(50); //Ajusta o contraste do display
  display.clearDisplay();   //Apaga o buffer e o display
  display.setTextSize(1);  //Seta o tamanho do texto
  display.setTextColor(BLACK); //Seta a cor do texto
  display.setCursor(0,0);  //Seta a posição do cursor
  //display.println("Menu Principal");  //tava arduino e cia
  display_print(0);
  display.display();

  /**
   * Inicializa as portas para a leitura do teclado matricial
   */
  pinMode(C1, INPUT_PULLUP);
  pinMode(C2, INPUT_PULLUP);
  pinMode(C3, INPUT_PULLUP);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);

  momento_pressionado = 0;

  /*
   * Inicializa a interrupcao de tempo a cada 10ms
   */
  Timer1.initialize(10000); 
  Timer1.attachInterrupt(contador_tempo);
  
  /*
   * Estado inicial = 150: mostra o menu principal
   */
  estado = 150;

  //Inicializa o I2C para a memoria 
  Wire.begin();
}


void loop() {

  //display_print(0);
  
  /* Lendo o teclado para o primeiro dígito */
  int teclado = checa_tecla();  //chamava teclado
  
  solto = 0;
//  Serial.println("teclado");
//  Serial.println(teclado);
  
  //pressionou a primeira tecla
  if(teclado != -1 && pressionou == false && lendo_tecla == 0) {
    if (teclado != 10 && teclado != 12) {
      if (teclado == 11)
        teclado = 0;
      Serial.println("leu a primeira tecla");
      Serial.println(teclado);
      digito_1 = teclado;
      
      pressionou = true;
      lendo_tecla = 1;
    }  
  }
  
  //soltou a primeira tecla
  if(teclado == -1 && pressionou == true && lendo_tecla == 1) {
//    Serial.println("soltou a tecla");
//    Serial.println(teclado);
    pressionou = false;
  }
  
  //pressionou a segunda tecla
  if(teclado != -1 && pressionou == false && lendo_tecla == 1) {
    if (teclado == 11)
        teclado = 0;
    Serial.println("leu a segunda tecla");
    Serial.println(teclado);
    digito_2 = teclado;
    pressionou = true;
    lendo_tecla = 2;
  }
  //soltou a primeira tecla
  if(teclado == -1 && pressionou == true && lendo_tecla == 2) {
//    Serial.println("soltou a tecla");
//    Serial.println(teclado);
    pressionou = false;
    lendo_tecla = 0;
    numero = 10*digito_1 + digito_2;
    Serial.println("Numero inserido: ");
    Serial.println(numero);
  }
  
  

  
  
  
}



/*
 * Funcao para tratamento da interrupção do TimerOne
 * incrementa os contadores e a cada 1 segundo atualiza o valor da temperatura
 */
void contador_tempo () {
  momento_pressionado += 1;
  tempo_display += 1;
}



