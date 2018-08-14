

void setup() {
	/*Estados:
	* 0 - carro: verde, pedestre: vermelho
	* 1 - carro: piscando amarelo, pedestre: piscando vermelho
	* 2 - carro: amarelo, pedestre: vermelho
	* 3 - carro: vermelho, pedestre: verde
	* 4 - carro: vermelho, pedestre: piscando vermelho
	*/
	int estado = 0;
	
	//Pinos Digitais
	int verde_carros = 5;
	int amarelo_carros = 6;
	int vermelho_carros = 7;
	int verde_pedestres = 8;
	int vermelho_pedestres = 9;
	int botao = 3; //por causa da funcao de interrupção externa do arduino

	//Pino Analogico
	int sensor_luz = 0;

	//Flags setadas pelas interrupções
	int flag_botao = 0;
	int flag_timer = 0;

	//Variaveis usadas no codigo
	int val_sensor_luz;

	pinMode(botao, INPUT);
  	pinMode(verde_carros, OUTPUT); // verde
 	pinMode(amarelo_carros, OUTPUT); // amarelo
  	pinMode(vermelho_carros, OUTPUT); // vermelho
  	pinMode(verde_pedestres, OUTPUT); //verde pedestre
  	pinMode(vermelho_pedestre, OUTPUT); //vermelho pedestre

  	//Serial para depuração
  	Serial.begin(9600);
}

void loop() {


  if (estado == 0) {
    //se estiver de dia
    digitalWrite(verde_carros, HIGH);
    digitalWrite(amarelo_carros, LOW);
    digitalWrite(vermelho_carros, LOW);
    digitalWrite(verde_pedestres, LOW);
    digitalWrite(vermelho_pedestres, HIGH);
    //interrupções verificando o pulsar do botão, caso for pressionado, chama função pra realizar as mudanças de estado pro pedestre poder passar.
    //fecha_semaforo();

  }
  else if (estado == 1) {
    //de noite
    //pisca amarelo pros carros
    //pisca vermelhor pro pedestre

    //apertar botão não faz efeito
  }

}

void seta_flag_botao(){
	flag_botao = 1;
}


void fecha_semaforo() {
  //espera 5 segundos
  //amarelo pros carros
  digitalWrite(verde_carros, LOW);
  digitalWrite(amarelo_carros, HIGH);
  //mais 5 segundos:
  //fecha pros carros e abre pro pedestre
  digitalWrite(amarelo_carros, LOW);
  digitalWrite(vermelho_carros, HIGH);
  digitalWrite(verde_pedestres, HIGH);
  //20 segundos
  digitalWrite(verde_pedestres, LOW);
  //pisca o vermelho do pedestre

  //5 segundos
  digitalWrite(vermelho_carros, LOW);
  digitalWrite(vermelho_pedestres, HIGH);
  digitalWrite(verde_carros, HIGH);
}

