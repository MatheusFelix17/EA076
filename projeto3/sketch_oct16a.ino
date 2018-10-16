/*Bibliotecas para a interface com o monitor LCD*/
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <TimerOne.h>


Adafruit_PCD8544 display = Adafruit_PCD8544(8, 9, 2); //

/*
 * Sensor de temperatura 
 */
int sensorTemperatura = A0;
float temperatura;

/*
 * Teclado Matricial
 */
int C1 = A3;
int C2 = A2;
int C3 = A1;
int L1 = 7;
int L2 = 6;
int L3 = 5;
int L4 = 4;

int estado;

// flags para detecção do botão:
int solto = 0, pressionado = 0;

// tempo em que uma tecla foi acionada, para fazer o debounce
unsigned long int momento_pressionado;
unsigned int intervaloTemperatura;
unsigned int tempo_display;
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

  pinMode(C1, INPUT_PULLUP);
  pinMode(C2, INPUT_PULLUP);
  pinMode(C3, INPUT_PULLUP);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);

  momento_pressionado = 0;
  //Inicializando o Timer para interromper a cada 10ms
  Timer1.initialize(10000); 
  Timer1.attachInterrupt(contador_tempo);
  
  //estado inicial depois vou mudar, coloquei 11 pra como se apertasse o 0 levasse ate o menu principal
  estado = 11;

}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(checa_tecla());
  //solto = 0;
  display.display();
  /*Estados:
  * 0 - TELA PRINCIPAL
  * 1 - 
  * 2 - 
  * 3 - 
  * 4 - 
  * 5 - 
  * 6 - 
  */
  estado = checa_tecla();
  solto = 0;
  
  //medeTemperatura();

  
  //Serial.print("Estado = ");
  //Serial.println(estado);

  //estado 'sem escolha' = -1
  
  switch(estado) {
    case 11:      
      //depois vou mudar, coloquei 11 pra como se apertasse o 0 levasse ate o menu principal
      Serial.println("Menu Principal");
      display_print(0);
      //CONSERTAR DEBOUNCE
      //sem o delay ficou aparecendo várias vezes na tela... corrigir isso
    break;

    case 1:
      //função de reset
      Serial.println("Quer resetar?");
      //fiz um 'tem certeza' genérico
      display_print(1);
      //se apertar o '#' que no teclado lemos como 12, aí confirma o comando
      while (estado != 12 && estado != 10) {
        //a confirmação vai ser feita se cancelar, com '*', ou confirmar, com '#'
        estado = checa_tecla();
        solto = 0;
      }  
      if (estado == 12) {
        Serial.println("Estou entrando na operacao de RESET");
        //função de reset
        //coloco uma confirmação de tela por alguns segundos e depois retorno pro Menu??
        display_print(11);
      }
      else if (estado == 10) {
        Serial.println("Voltando ao MENU");
        //* cancelou, entao volta a printar o menu
        display_print(0);
      }
    break;

    case 2:
      //função de measure
      Serial.println("Farei as medidas");
      display_print(1);
      while (estado != 12 && estado != 10) {
        //a confirmação vai ser feita se cancelar, com '*', ou confirmar, com '#'
        estado = checa_tecla();
        solto = 0;
      }  
      if (estado == 12) {
        Serial.println("Estou entrando na operacao de MEASURE");
        display_print(2);
        tempo_display = 0;
        //função 
      }
      else if (estado == 10) {
        Serial.println("Voltando ao MENU");
        //* cancelou, entao volta a printar o menu
        display_print(0);
      }      
    break;

    case 3:
      //função this_status();
      Serial.println("Status:   ...");
      display_print(1);
      while (estado != 12 && estado != 10) {
        //a confirmação vai ser feita se cancelar, com '*', ou confirmar, com '#'
        estado = checa_tecla();
        solto = 0;
      }  
      if (estado == 12) {
        Serial.println("Estou entrando na operacao de STATUS");
        display_print(3);
        //função 
      }    
      else if (estado == 10) {
        Serial.println("Voltando ao MENU");
        //* cancelou, entao volta a printar o menu
        display_print(0);
      }        
    break;

    case 4:
      //start_collect();
      Serial.println("Iniciando Coleta de dados...");
      display_print(1);
      while (estado != 12 && estado != 10) {
        //a confirmação vai ser feita se cancelar, com '*', ou confirmar, com '#'
        estado = checa_tecla();
        solto = 0;
      }  
      if (estado == 12) {        
        Serial.println("Estou entrando na operacao de INICIAR COLETA");
        display_print(4);
        //função 
      }   
      else if (estado == 10) {
        Serial.println("Voltando ao MENU");
        //* cancelou, entao volta a printar o menu
        display_print(0);
      }           
    break;

    case 5:
      //stop_collect();
      Serial.println("Finalizando coleta de dados");
      display_print(1);
      while (estado != 12 && estado != 10) {
        //a confirmação vai ser feita se cancelar, com '*', ou confirmar, com '#'
        estado = checa_tecla();
        solto = 0;
      }  
      if (estado == 12) {
        Serial.println("Estou entrando na operacao de ENCERRAR COLETA");
        display_print(5);
        //função 
      }  
      else if (estado == 10) {
        Serial.println("Voltando ao MENU");
        //* cancelou, entao volta a printar o menu
        display_print(0);
      }            
    break;

    case 6:
      //transfer_data();
      Serial.println("Transferindo dados");
      display_print(1);
      while (estado != 12 && estado != 10) {
        //a confirmação vai ser feita se cancelar, com '*', ou confirmar, com '#'
        estado = checa_tecla();
        solto = 0;
      }  
      if (estado == 12) {
        Serial.println("Estou entrando na operacao de TRANSFERIR DADOS");
        display_print(6);
        //função 
      }   
      else if (estado == 10) {
        Serial.println("Voltando ao MENU");
        //* cancelou, entao volta a printar o menu
        display_print(0);
      }           
    break;

    default:
    break;
  }
  
  
}

void display_print(int i) {
  switch(i) {

    /* display.begin();
    display.setContrast(40); //Ajusta o contraste do display
    display.clearDisplay();   //Apaga o buffer e o display
    display.setTextSize(1);  //Seta o tamanho do texto
    display.setTextColor(BLACK); //Seta a cor do texto
    display.setCursor(0,0);  //Seta a posição do cursor
    display.println("Menu Principal");
    display.display(); */
  
    //menu principal
    case 0:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("1 - Reset");
      display.setCursor(0,8);
      display.println("2 - Measure");
      display.setCursor(0,16);
      display.println("3 - Status");
      display.setCursor(0,24);
      display.println("4 - Start");
      display.setCursor(0,32);
      display.println("5 - End");
      display.setCursor(0,40);
      display.println("6 - Transfer");
      display.display();
    break;
  
    case 1:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Tem Certeza?");
      display.setCursor(0,8);
      display.println("* para cancelar");
      display.setCursor(0,16);
      display.println("# para confirmar");
      display.display();
    break;
  
    case 2:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Temperatura: ");
      //tem mais coisa daqui
      display.setCursor(0,16);
      display.setTextSize(2);
      display.println(temperatura);
      display.display();
      display.setTextSize(1);
      if (tempo_display > 200) {
        estado = 11;  //mudar para -1
      }
    break;
  
    case 3:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Dados gravados: ");
      //print do numero
      display.setCursor(0,8);
      display.println("Medicoes disponiveis: ");
      //print do numero
      display.display();
    break;
  
    case 4:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Iniciando coleta periodica");
      display.display();
    break;
  
    case 5:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Coleta periodica encerrada");
      display.display();
    break;
  
    case 6:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Insira o numero de medicoes");  //quantas quer transferir
      display.setCursor(0,16);
      display.println("Seguido de * ou #");
      display.display();
    break;

    case 11:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Memoria APAGADA");
    break;
  
    default:
    break;

  }
  
}

int checa_coluna() {
  int j;

  if (digitalRead(C1)== LOW){
    return 1;
  }
  if (digitalRead(C2)== LOW){
    return 2;
  }
  if (digitalRead(C3)== LOW){
    return 3;
  }

  return -1;
}

void contador_tempo () {
  momento_pressionado += 1;
  intervaloTemperatura += 1;
  tempo_display += 1;
  if (intervaloTemperatura > 200){//passou 2s
    intervaloTemperatura = 0;
    temperatura = (analogRead(sensorTemperatura)/1023.0)*5*100;
  }
}

//void medeTemperatura(){
//  temperatura = (analogRead(sensorTemperatura)/1023.0)*5*100;
//}


int checa_tecla() {
  int i, coluna_pressionada;
  int linha_pressionada = 0;
  digitalWrite(L1, HIGH);
  digitalWrite(L2, HIGH);
  digitalWrite(L3, HIGH);
  digitalWrite(L4, HIGH);
  //Varre todas as linhas
  for (i = 0; i < 4 && solto == 0; i++) {
     switch (i) {
      case 0:
        digitalWrite(L1, LOW);
        coluna_pressionada = checa_coluna();
        //Detectou que tem tecla pressionada
        if (coluna_pressionada != -1){
          if (pressionado == 0) {
            momento_pressionado = 0;
            pressionado = 1;
          }
          //TEMPO PARA DEBOUNCE
          if (pressionado == 1 && momento_pressionado >= 3) { //30 ms
            solto = 1;
            linha_pressionada = i;
            return linha_pressionada * 3 + coluna_pressionada;
          } 
        }
        digitalWrite(L1, HIGH);
      break;

      case 1:
        
        digitalWrite(L2, LOW);
        coluna_pressionada = checa_coluna();
        //Detectou que tem tecla pressionada
        if (coluna_pressionada != -1){
          if (pressionado == 0) {
            momento_pressionado = 0;
            pressionado = 1;
          }
          //TEMPO PARA DEBOUNCE
          if (pressionado == 1 && momento_pressionado >= 3) { //30ms
            solto = 1;
            linha_pressionada = i;
            return linha_pressionada * 3 + coluna_pressionada; 
          }          
        }
        digitalWrite(L2, HIGH);
      break;

      case 2:
        digitalWrite(L3, LOW);
        coluna_pressionada = checa_coluna();
        //Detectou que tem tecla pressionada
        if (coluna_pressionada != -1){
          if (pressionado == 0) {
            momento_pressionado = 0;
            pressionado = 1;
          }
          //TEMPO PARA DEBOUNCE
          if (pressionado == 1 && momento_pressionado >= 3) { //30ms
            solto = 1;
            linha_pressionada = i;
            return linha_pressionada * 3 + coluna_pressionada; 
          } 
        }
        digitalWrite(L3, HIGH);
      break;  

      case 3:
        digitalWrite(L4, LOW);
        coluna_pressionada = checa_coluna();
        //Detectou que tem tecla pressionada
        if (coluna_pressionada != -1){
          if (pressionado == 0) {
            momento_pressionado = 0;
            pressionado = 1;
          }
          //TEMPO PARA DEBOUNCE
          if (pressionado == 1 && momento_pressionado >= 3) { //30ms
            solto = 1;
            linha_pressionada = i;
            return linha_pressionada * 3 + coluna_pressionada; 
          }          
        }
        digitalWrite(L4, HIGH);
      break;

      default:
      break;
      
     }//end switch
  }//end for
  return -1;
}
