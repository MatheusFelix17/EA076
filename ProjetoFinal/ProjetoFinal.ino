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

bool primeiro_voto[8]; //caso seja o primeiro voto e a memória não está zerada, 7 candidatos + posicao nula, = 100

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);

  //cada voto que for realizado para cada candidato será o primeiro (até que se repita o numero)
  for (int i = 0; i < 8; i++)
    primeiro_voto[i] = true;

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
      display_print(100); //confirmacao de voto
      display.display();
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
 * Função que realiza a contagem dos votos do candidato
 * na memória EEPROM
 * 
 */

void salva_voto(int numero) {
  int atual;
  
  /*
   * recebe o numero do candidato,
   * faz a leitura correspondente do numero de votos
   * que esse candidato possui atualmente,
   * incrementa 1, e salva novamente na posicao de memória
   */
   //posicao do candidato na memória = 2*numero do candidato

  switch(numero) {
    case 11:
      atual = readEeprom(22);
      if (primeiro_voto[0] == true) { //se for o primeiro voto e tiver algum valor diferente de 0 lá, consideramos 0
        if (atual != 0) {
          atual = 0;         
        }
        primeiro_voto[0] = false;
      }
      
      atual += 1; //soma 1 voto e salva na posicao correspondente
      writeEepromPosition(22, atual);
          
    break;

    case 17:
      atual = readEeprom(34);
      if (primeiro_voto[1] = true) {
        if (atual != 0)
          atual = 0;
        primeiro_voto[1] = false;  
      }

      atual += 1;
      writeEepromPosition(34, atual);

    break;

    case 22:
      atual = readEeprom(44);
      if (primeiro_voto[2] = true) {
        if (atual != 0)
          atual = 0;
        primeiro_voto[2] = false;  
      }

      atual += 1;
      writeEepromPosition(44, atual);    

    break;

    case 33:
      atual = readEeprom(66);
      if (primeiro_voto[3] = true) {
        if (atual != 0)
          atual = 0;
        primeiro_voto[3] = false;  
      }

      atual += 1;
      writeEepromPosition(66, atual);    

    break;

    case 44:
      atual = readEeprom(88);
      if (primeiro_voto[4] = true) {
        if (atual != 0)
          atual = 0;
        primeiro_voto[4] = false;  
      }

      atual += 1;
      writeEepromPosition(88, atual);    

    break;

    case 55:
      atual = readEeprom(110);
      if (primeiro_voto[5] = true) {
        if (atual != 0)
          atual = 0;
        primeiro_voto[5] = false;  
      }

      atual += 1;
      writeEepromPosition(110, atual);    

    break;

    case 66:
      atual = readEeprom(132);
      if (primeiro_voto[6] = true) {
        if (atual != 0)
          atual = 0;
        primeiro_voto[6] = false;  
      }

      atual += 1;
      writeEepromPosition(132, atual);    

    break;

    default:
      atual = readEeprom(200);
      if (primeiro_voto[7] = true) {
        if (atual != 0)
          atual = 0;
        primeiro_voto[7] = false;  
      }

      atual += 1;
      writeEepromPosition(200, atual);
    break;
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



