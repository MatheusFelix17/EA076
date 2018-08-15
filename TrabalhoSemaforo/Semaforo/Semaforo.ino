#include <TimerOne.h>

#define LIMIAR 300

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
int flag_temporaria;

int valores_sensor_luz[30]; 
volatile unsigned long int tempo;
volatile unsigned long int tempo_botao;
volatile unsigned long int tempo_luz;
int i;
int alternador_vermelho_pedestre;
int alternador_amarelo_carro;
int contador_piscadas;

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
	flag_temporaria= 0;

	//Inicializa os tempos
	tempo = 0;
	tempo_botao = 0;
	tempo_luz = 0;


	//Inicializando a direçao dos pinos da GPIO
  	pinMode(botao, INPUT);
    pinMode(verde_carros, OUTPUT); // verde
   	pinMode(amarelo_carros, OUTPUT); // amarelo
  	pinMode(vermelho_carros, OUTPUT); // vermelho
  	pinMode(verde_pedestres, OUTPUT); //verde pedestre
  	pinMode(vermelho_pedestres, OUTPUT); //vermelho pedestre

  	//Inicializando o Timer para interromper a cada 10ms
  	Timer1.initialize(10000); 
  	Timer1.attachInterrupt(contador_tempo);

  	//Inicializando a interrupção com o pushbutton
  	//attachInterrupt(digitalPinToInterrupt(3), seta_flag_botao, RISING);

  	//Serial para depuração
  	Serial.begin(9600);

  	//Inicializando os leds para o estado 0 (verde para carros)
  	digitalWrite(verde_carros, HIGH);
	digitalWrite(amarelo_carros, LOW);
	digitalWrite(vermelho_carros, LOW);
	digitalWrite(verde_pedestres, LOW);
	digitalWrite(vermelho_pedestres,HIGH);

	alternador_vermelho_pedestre = 1;
	alternador_amarelo_carro = 1;
	contador_piscadas = 0;


	//Inicializa o vetor de valores de luminosidade
	for (i = 0; i < 30; i++){
		valores_sensor_luz[i] = 0;
	}
	int i = 0;
}

void loop() {
  	//Serial.println(tempo);
  	Serial.println(estado);

  	checa_luz();

	//maquina de estados
	switch(estado){
		case 0:
			checa_botao();
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
		// case 4:
		// 	transicao();
		// 	flag_botao = 0;
		// 	break;
		case 4:
			semaforo_piscando_noite();
			flag_botao = 0;
			break;
		default:
			break;
	}

}


void contador_tempo (){
  tempo += 1;
  tempo_botao += 1;
  tempo_luz += 1;
}


//pode ter erro aqui <-- cuidado
void checa_botao(){
	if (digitalRead(botao) && (flag_temporaria == 0)){
    Serial.println("detectou a primeira borda");
  		flag_temporaria = 1;
  		tempo_botao = 0 ;
  	}
  	if (tempo_botao > 3 && (flag_temporaria == 1)){//se passou 30ms
  		flag_temporaria = 2;
  		if (digitalRead(botao)){
        Serial.println("continua apertado");
  			flag_botao = 1;
  			tempo = 0;
  		}
  	}

  	else if (tempo_botao > 401 && (flag_temporaria > 0) ){
  		flag_temporaria = 0;
  	}
  
}

void checa_luz(){
	//medir a luz de 100ms em 100ms
	if (tempo_luz >= 10){
		valores_sensor_luz[i] = analogRead(sensor_luz);
		i = (i + 1)%30;
		tempo_luz = 0;
	}
}

int calcula_media_luz(){
	float media;
	for (int j = 0 ; j < 30; j++){
		media += valores_sensor_luz[i];
	}
	return (int)(media/30);
}


void semaforo_carro_aberto(){
	//se ficou escuro
	if (calcula_media_luz() > LIMIAR){
		flag_botao = 0;
		// tempo_antes = tempo;
		tempo = 0;
		estado = 4;
	}
	//se apertou o botao
	if (tempo > 400 && flag_botao){
		flag_botao = 0;
		// tempo_antes = tempo;
		tempo = 0;
		estado = 1;
		digitalWrite(verde_carros, LOW);
		digitalWrite(amarelo_carros, HIGH);
		digitalWrite(vermelho_carros, LOW);
		digitalWrite(verde_pedestres, LOW);
		digitalWrite(vermelho_pedestres,HIGH);
	}
}

void semaforo_carro_amarelo(){
	
	//se tiver passado 5 segundos
	if (tempo > 500 ){
		// tempo_antes = tempo;
		tempo = 0;
		estado = 2;
		digitalWrite(verde_carros, LOW);
		digitalWrite(amarelo_carros, LOW);
		digitalWrite(vermelho_carros, HIGH);
		digitalWrite(verde_pedestres, HIGH);
		digitalWrite(vermelho_pedestres,LOW);
	}
}

void semaforo_carro_vermelho(){
	
	//se tiver passado 15 segundos
	if (tempo > 1500 ){
		// tempo_antes = tempo;
		tempo = 0;
		estado = 3;
		digitalWrite(verde_carros, LOW);
		digitalWrite(amarelo_carros, LOW);
		digitalWrite(vermelho_carros, HIGH);
		digitalWrite(verde_pedestres, LOW);
		digitalWrite(vermelho_pedestres,LOW);
	}
}

void semaforo_pedestre_piscando(){
	
	//pisca o vermelho (inverte seu valor logico a cada 0.5s)
	if (tempo > 50){
		// tempo_antes = tempo;
		tempo = 0;
		alternador_vermelho_pedestre = 1 - alternador_vermelho_pedestre;
		digitalWrite(vermelho_pedestres, alternador_vermelho_pedestre);
		contador_piscadas += 1;
	}
	//se tiver passado 5 segundos
	if (contador_piscadas >= 10 ){
		// tempo_antes = tempo;
		tempo = 0;
		estado = 0;
		contador_piscadas = 0;
		digitalWrite(verde_carros, HIGH);
		digitalWrite(amarelo_carros, LOW);
		digitalWrite(vermelho_carros, LOW);
		digitalWrite(verde_pedestres, LOW);
		digitalWrite(vermelho_pedestres,HIGH);
	}
}

// void transicao(){
// 	//se passou 5s
// 	if (tempo > 500){
// 		//se esta claro, volta para o estado 0 (verde para os carros)
// 		if (analogRead(sensor_luz) < LIMIAR){
// 			// tempo_antes = tempo;
// 			tempo = 0;
// 			estado = 0;
// 			digitalWrite(verde_carros, HIGH);
// 			digitalWrite(amarelo_carros, LOW);
// 			digitalWrite(vermelho_carros, LOW);
// 			digitalWrite(verde_pedestres, LOW);
// 			digitalWrite(vermelho_pedestres,HIGH);
// 		}
// 		//senao, se esta escuro vai para o estado 
// 		else{
// 			// tempo_antes = tempo;
// 			tempo = 0;
// 			estado = 5;
// 			digitalWrite(verde_carros, LOW);
// 			digitalWrite(amarelo_carros, HIGH);
// 			digitalWrite(vermelho_carros, LOW);
// 			digitalWrite(verde_pedestres, LOW);
// 			digitalWrite(vermelho_pedestres,HIGH);
// 		}
// 	}

// }

void semaforo_piscando_noite(){
	//pisca o vermelho (inverte seu valor logico a cada 0.5s)
	if (tempo > 50){
		// tempo_antes = tempo;
		tempo = 0;
		alternador_vermelho_pedestre = 1 - alternador_vermelho_pedestre;
		alternador_amarelo_carro = 1 - alternador_amarelo_carro;
		digitalWrite(vermelho_pedestres, alternador_vermelho_pedestre);
		digitalWrite(amarelo_carros, alternador_amarelo_carro);
	}
	//se fica claro, vai para o estado de transicao entre dia e noite
	if (calcula_media_luz() < LIMIAR){
		// tempo_antes = tempo;
		tempo = 0;
		estado = 0;
		digitalWrite(verde_carros, LOW);
		digitalWrite(amarelo_carros, HIGH);
		digitalWrite(vermelho_carros, LOW);
		digitalWrite(verde_pedestres, LOW);
		digitalWrite(vermelho_pedestres,HIGH);
	}

}
