/*Bibliotecas para a interface com o monitor LCD*/
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <TimerOne.h>
#include <Wire.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(8, 9, 2); //

/*
 * Sensor de temperatura 
 */
int sensorTemperatura = A0;
float temperatura;

/*
 * Teclado Matriciala
 */
int C1 = A3;
int C2 = A2;
int C3 = A1;
int L1 = 7;
int L2 = 6;
int L3 = 5;
int L4 = 4;

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
unsigned short intervaloTemperatura;
unsigned short tempo_display;
unsigned short tempoColeta;


bool flagColeta = false;


void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);

  temperatura = 0;

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

  //Modificação da referencia do conversor AD para maior precisao
  analogReference(INTERNAL);
}


void loop() {

  display.display();

  /*
   * Le o teclado
   */
  int teclado = checa_tecla();
  solto = 0;

  /*
   * Se o teclado tem um valor valido (-1 significa que nenhuma tecla esta apertada)
   * e ele nao esta no estado transitorio entre estados
   * o estado muda para o estado digitado
   */
  if (teclado != -1 && estado != 100){
    estado = teclado;
  }

  /*
   * Se esta acontecendo a coleta automatica e o contador de tempo mostra que passou 2s
   * escreve na memoria o valor de temperatura lido
   */
  if (flagColeta == true && tempoColeta > 200){//se passou 2 segundos e tem que escrever a temperatura na memoria
    tempoColeta = 0;
    writeEeprom(temperatura * 10);
  }

  /*
   * Maquina de estados
   */
  switch(estado) {
    
    //MENU PRINCIPAL
    case 150:      
      display_print(0);
    break;

    //APERTOU 1 : Reset
    case 1:
      display_print(1);
      //aguarda apertar '*' ou '#' 
      while (estado != 12 && estado != 10) {
        //a confirmação vai ser feita se cancelar, com '*', ou confirmar, com '#'
        estado = checa_tecla();
        solto = 0;
      }  
      
      //se apertar o '#' que no teclado lemos como 12, resetamos
      if (estado == 12) {
        resetEeprom();
        display_print(11);
        tempo_display = 0;
        estado = 100;
        
      }
      
      else if (estado == 10) {
        //* cancelou, entao volta a printar o menu
        display_print(0);
        estado = 150;
      }
    break;

    //APERTOU 2 : Mostra a temperatura
    case 2:
      display_print(1);
      while (estado != 12 && estado != 10) {
        //a confirmação vai ser feita se cancelar, com '*', ou confirmar, com '#'
        estado = checa_tecla();
        solto = 0;
      }  
      if (estado == 12) {
        display_print(2);
        tempo_display = 0;
        estado = 100;
      }
      
      else if (estado == 10) {
        //* cancelou, entao volta a printar o menu
        display_print(0);
        estado = 150;
      }      
    break;

    //APERTOU 3 : Mostra Status
    case 3:
      display_print(1);
      while (estado != 12 && estado != 10) {
        //a confirmação vai ser feita se cancelar, com '*', ou confirmar, com '#'
        estado = checa_tecla();
        solto = 0;
      }  
      if (estado == 12) {
        display_print(3);
        tempo_display = 0;
        estado = 100; 
      }    
      else if (estado == 10) {
        //* cancelou, entao volta a printar o menu
        display_print(0);
        estado = 150;
      }        
    break;

    //APERTOU 4 : Começa coleta periódica
    case 4:
      //start_collect();
      //Serial.println("Iniciando Coleta de dados...");
      display_print(1);
      while (estado != 12 && estado != 10) {
        //a confirmação vai ser feita se cancelar, com '*', ou confirmar, com '#'
        estado = checa_tecla();
        solto = 0;
      }  
      if (estado == 12) {        
        //Serial.println("Estou entrando na operacao de INICIAR COLETA");
        display_print(4);
        flagColeta = true;
        tempoColeta = 0;
        tempo_display = 0;
        estado = 100;
      }   
      else if (estado == 10) {
        //Serial.println("Voltando ao MENU");
        //* cancelou, entao volta a printar o menu
        display_print(0);
        estado = 150;
      }           
    break;

    //APERTOU 5 : Para a coleta periódica
    case 5:
      display_print(1);
      while (estado != 12 && estado != 10) {
        //a confirmação vai ser feita se cancelar, com '*', ou confirmar, com '#'
        estado = checa_tecla();
        solto = 0;
      }  
      if (estado == 12) {
        display_print(5);
        tempo_display = 0;
        flagColeta = false;
        estado = 100;
      }  
      else if (estado == 10) {
        //Serial.println("Voltando ao MENU");
        //* cancelou, entao volta a printar o menu
        display_print(0);
        estado = 150;
      }            
    break;

    //APERTOU 6 : Imprime valores desejados
    case 6:
      display_print(1);
      while (estado != 12 && estado != 10) {
        //a confirmação vai ser feita se cancelar, com '*', ou confirmar, com '#'
        estado = checa_tecla();
        solto = 0;
      }  
      if (estado == 12) {
        display_print(6);
        transferencia();
      }   
      else if (estado == 10) {
        //* cancelou, entao volta a printar o menu
        display_print(0);
        estado = 150;
      }           
    break;

    //Estado transitorio (espera 2s para a transição entre estados)
    case 100:
      if (tempo_display > 200){
        estado = 150; 
      }
    break;

    default:
    break;
  }
  
  
}


/*
 * Funcao para tratamento da interrupção do TimerOne
 * incrementa os contadores e a cada 1 segundo atualiza o valor da temperatura
 */
void contador_tempo () {
  momento_pressionado += 1;
  intervaloTemperatura += 1;
  tempo_display += 1;
  tempoColeta += 1;
  if (intervaloTemperatura > 100){//passou 2s
    intervaloTemperatura = 0;
    temperatura = (analogRead(sensorTemperatura)/1023.0)*1.1*100;
  }
}


/*
 * Funcao para o tratamento do estado 6, se queremos imprimir os valores armazenados
 */
void transferencia(){
  
  String valor = "";
  bool flagNumeroUnico = false;
  int tecla = 0;
  tecla = checa_tecla();
  int apertado = 0;
  solto = 0;
  
  //Ignorando o # da confirmacao da mudança de estado
  while (tecla == 12){
    tecla = checa_tecla();
    solto = 0;
  }

  //Enquanto nao aperta # ou *
  while (tecla != 12 && tecla != 10) {
    tecla = checa_tecla();
    solto = 0;
    //Se a tecla pressionada for diferente (no caso, se soltar ou apertar uma nova tecla
    if (apertado != tecla){
      apertado = tecla;
      //Salva o valor pressionado em uma string
      if (apertado != -1 && apertado != 10 && apertado != 12){
        //salvar na string
        switch (apertado){
          case 1:
            valor = valor + "1";
          break;
          case 2:
            valor = valor + "2";
          break;
          case 3:
            valor = valor + "3";
          break;
          case 4:
            valor = valor + "4";
          break;
          case 5:
            valor = valor + "5";
          break;
          case 6:
            valor = valor + "6";
          break;
          case 7:
            valor = valor + "7";
          break;
          case 8:
            valor = valor + "8";
          break;
          case 9:
            valor = valor + "9";
          break;
          case 11:
            valor = valor + "0";
          break;
          default:
          break;
          
        }
      }
    }
  }    

  if (tecla == 12){//se confirmou
    if (valor.toInt() >=1 && valor.toInt() <=1022){//se pediu uma quantidade valida
      if (valor.toInt() <= memoriaLivre(2046)){//se a quantidade pedida é menor ou igual ao valor que contem na memoria
        for (int i = 0; i < valor.toInt(); i++){
          Serial.println(readEeprom(i*2)*0.1);
        }
      }
      else{//se o valor pedido excede a quantidade de valores salvos na memoria
        Serial.println("VALOR QUE EXCEDE O MAXIMO ARMAZENADO, IMPRIMINDO OS VALORES ARMAZENADOS EXISTENTES");
        for (int i = 0; i < memoriaLivre(2046); i++){
          Serial.println(readEeprom(i*2)*0.1);
        }
      }
    }
    else//Se pediu um valor invalido
      Serial.println("VALOR INVALIDO, POR FAVOR TENTE NOVAMENTE COM UM VALOR ENTRE 1 E 1022");
  }
  tempo_display = 0;
  estado = 100;
}

