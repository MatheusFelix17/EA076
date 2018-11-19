/*Bibliotecas para a interface com o monitor LCD*/
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <TimerOne.h>
#include <Wire.h>


/*
   Teclado Matricial
*/
#define C1 6
#define C2 5
#define C3 4
#define L1 A0
#define L2 A1
#define L3 A2
#define L4 A3


Adafruit_PCD8544 display = Adafruit_PCD8544(9, 8, 7); //


// flags para detecção do botão:
short solto = 0, pressionado = 0;

/**
   Contadores de tempo
*/
unsigned short momento_pressionado;
unsigned short tempo_display;

/*flag para leitura de teclado*/
bool pressionou = false;
bool soltou = true;
bool imprimiu_visor = false;
int lendo_tecla = 0;
int digito_1, digito_2;
int numero;

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);

  /*
     Inicializa o Visor LCD
  */
  display.begin();
  display.setContrast(50); //Ajusta o contraste do display
  display.clearDisplay();   //Apaga o buffer e o display
  display.setTextSize(1);  //Seta o tamanho do texto
  display.setTextColor(BLACK); //Seta a cor do texto
  display.setCursor(0, 0); //Seta a posição do cursor
  display_print(0);
  display.display();

  /**
     Inicializa as portas para a leitura do teclado matricial
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
     Inicializa a interrupcao de tempo a cada 10ms
  */
  Timer1.initialize(10000);
  Timer1.attachInterrupt(contador_tempo);

  //Inicializa o I2C para a memoria
  Wire.begin();
}


void loop() {

  /* Lendo o teclado para o primeiro dígito */
  int teclado = checa_tecla();  //chamava teclado

  solto = 0;

  //pressionou a primeira tecla
  if (teclado != -1 && pressionou == false && lendo_tecla == 0) {
    if (teclado != 10 && teclado != 12) {
      if (teclado == 11)
        teclado = 0;
      Serial.println("leu a primeira tecla");
      Serial.println(teclado);
      digito_1 = teclado;

      pressionou = true;
      lendo_tecla = 1;
    }
    else{
      pressionou = false;
      lendo_tecla = 0;
    }
  }

  //soltou a primeira tecla
  if (teclado == -1 && pressionou == true && lendo_tecla == 1) {
    pressionou = false;
  }

  //pressionou a segunda tecla
  if (teclado != -1 && pressionou == false && lendo_tecla == 1) {
    if (teclado != 10 && teclado != 12) {
      if (teclado == 11)
        teclado = 0;
      Serial.println("leu a segunda tecla");
      Serial.println(teclado);
      digito_2 = teclado;
      pressionou = true;
      lendo_tecla = 2;
    }
    else{
      pressionou = false;
      lendo_tecla = 0;
    }
  }

  //soltou a segunda tecla
  if (teclado == -1 && pressionou == true && lendo_tecla == 2) {
    pressionou = false;
    numero = 10 * digito_1 + digito_2;
    Serial.println("Numero inserido: ");
    Serial.println(numero);
  }

  //imprime no visor depois de digitar todos os digitos
  if (pressionou == false && lendo_tecla == 2 && imprimiu_visor == false){
    Serial.println("Imprimindo no visor");
    imprimiu_visor = true;
    display_print(numero);
  }

  //confirma ou cancela
  if (teclado != -1 && pressionou == false && lendo_tecla == 2 && imprimiu_visor == true) {
    Serial.println("Numero inserido: ");
    Serial.println(teclado);
    imprimiu_visor = false;
    pressionou = true;
    lendo_tecla = 3;
    //cancela
    if (teclado == 10) {
      Serial.println("Cancelou");
    }
    //confirma
    else if (teclado == 12){
      Serial.println("Confirmou");
    }
  }

  //soltou a terceira tecla (confirmar ou cancelar)
  if (teclado == -1 && pressionou == true && lendo_tecla == 3) {
    pressionou = false;
    lendo_tecla = 0;
    display_print(0);
    display.display();
  }

  


}



/*
   Funcao para tratamento da interrupção do TimerOne
   incrementa os contadores e a cada 1 segundo atualiza o valor da temperatura
*/
void contador_tempo () {
  momento_pressionado += 1;
  tempo_display += 1;
}



