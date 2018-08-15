#include <TimerOne.h>

int estado = 0;
	
//Pinos Digitais
int verde_carros;
int amarelo_carros;
int vermelho_carros;
int verde_pedestres;
int vermelho_pedestres;
int botao;

//Pino Analogico
int sensor_luz;

//Flags setadas pelas interrupções
int flag_botao;

int val_sensor_luz;
unsigned int tempo;
unsigned int tempo_antes;

void setup() {
	/*Estados:
	* 0 - carro: verde, pedestre: vermelho
	* 1 - carro: amarelo, pedestre:  vermelho
	* 2 - carro: vermelho, pedestre: verde
	* 3 - carro: vermelho, pedestre: piscando vermelho
	* 4 - transicao dia e noite
	* 5 - carro: piscando amarelo, pedestre: piscando vermelho
	*/
	estado = 0;
	
	//Pinos Digitais
	verde_carros = 5;
	amarelo_carros = 6;
	vermelho_carros = 7;
	verde_pedestres = 8;
	vermelho_pedestres = 9;
	botao = 3; //por causa da funcao de interrupção externa do arduino

	//Pino Analogico
	sensor_luz = 5;

	//Flags setadas pelas interrupções
	flag_botao = 0;
	flag_timer = 0;

	//Inicializando a direçao dos pinos da GPIO
  	pinMode(botao, INPUT);
    pinMode(verde_carros, OUTPUT); // verde
   	pinMode(amarelo_carros, OUTPUT); // amarelo
  	pinMode(vermelho_carros, OUTPUT); // vermelho
  	pinMode(verde_pedestres, OUTPUT); //verde pedestre
  	pinMode(vermelho_pedestre, OUTPUT); //vermelho pedestre

  	//Inicializando o Timer para interromper a cada 10ms
  	Timer1.initialize(10000); 
  	Timer1.attachInterrupt(contador_tempo);

  	//Inicializando a interrupção com o pushbutton
  	attachInterrupt(digitalPinToInterrupt(3), seta_flag_botao, RISING);

  	//Serial para depuração
  	Serial.begin(9600);
}

void loop() {
	//maquina de estados
	switch(estado){
		case 0:
			semaforo_carro_aberto();
			break;
		case 1:
			semaforo_carro_amarelo();
			flag_botao = 0;
			break;
		case 2:
			semaforo_carro_vermelho();
			flag_botao = 0;
			break;
		case 3:
			semaforo_pedestre_piscando();
			flag_botao = 0;
			break;
		case 4:
			flag_botao = 0;
			break;
		case 5:
			flag_botao = 0;
			break;
		default:
			break;
	}

}


void contador_tempo (){
  tempo += 1;
}


void seta_flag_botao(){
	unsigned int tempo_aux = tempo;
	//evita debounce
	while(abs(tempo - tempo_aux) <  3);
	if (digitalRead(3) == 1){
		flag_botao = 1;
	}
}

void semaforo_carro_aberto(){
	digitalWrite(verde_carros, HIGH);
	digitalWrite(amarelo_carros, LOW);
	digitalWrite(vermelho_carros, LOW);
	digitalWrite(verde_pedestres, LOW);
	digitalWrite(vermelho_pedestres,HIGH);
	if (flag_botao){
		flag_botao = 0;
		tempo_antes = tempo;
		estado = 1;
	}
}

void semaforo_carro_amarelo(){
	digitalWrite(verde_carros, LOW);
	digitalWrite(amarelo_carros, HIGH);
	digitalWrite(vermelho_carros, LOW);
	digitalWrite(verde_pedestres, LOW);
	digitalWrite(vermelho_pedestres,HIGH);
	//se tiver passado 5 segundos
	if (abs(tempo - tempo_antes) > 500 ){
		tempo_antes = tempo;
		estado = 2;
	}
}

void semaforo_carro_vermelho(){
	digitalWrite(verde_carros, LOW);
	digitalWrite(amarelo_carros, LOW);
	digitalWrite(vermelho_carros, HIGH);
	digitalWrite(verde_pedestres, HIGH);
	digitalWrite(vermelho_pedestres,LOW);
	
	//se tiver passado 15 segundos
	if (abs(tempo - tempo_antes) > 1500 ){
		tempo_antes = tempo;
		estado = 3;
	}
}

void semaforo_pedestre_piscando(){
	digitalWrite(verde_carros, LOW);
	digitalWrite(amarelo_carros, LOW);
	digitalWrite(vermelho_carros, HIGH);
	digitalWrite(verde_pedestres, LOW);

	//pisca o vermelho (inverte seu valor logico a cada 0.5s)
	if (abs(tempo - tempo_antes) > 50){
		digitalWrite(vermelho_pedestres,!digitalRead(vermelho_pedestres));
	}
	//se tiver passado 5 segundos
	if (abs(tempo - tempo_antes) > 500 ){
		tempo_antes = tempo;
		estado = 0;
	}
}


// void fecha_semaforo() {
//   //espera 5 segundos
//   //amarelo pros carros
//   digitalWrite(verde_carros, LOW);
//   digitalWrite(amarelo_carros, HIGH);
//   //mais 5 segundos:
//   //fecha pros carros e abre pro pedestre
//   digitalWrite(amarelo_carros, LOW);
//   digitalWrite(vermelho_carros, HIGH);
//   digitalWrite(verde_pedestres, HIGH);
//   //20 segundos
//   digitalWrite(verde_pedestres, LOW);
//   //pisca o vermelho do pedestre

//   //5 segundos
//   digitalWrite(vermelho_carros, LOW);
//   digitalWrite(vermelho_pedestres, HIGH);
//   digitalWrite(verde_carros, HIGH);
// }

