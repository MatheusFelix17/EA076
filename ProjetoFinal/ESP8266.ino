
//Setando variaveis globais para teste do servidor
#define DEBUG true


/*FUNÇÃO PARA COMUNICAÇÃO SERIAL ENTRE ARDUINO E ESP8266*/

String sendData(String command, const int timeout, boolean debug = false){
  // Send AT commands
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis()){
    while (esp8266.available()){
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }
  if (debug){
    Serial.println(response);
  }
  return response;
}

/*FUNÇÃO PARA POST DE UMA PAGINA NO SERVIDOR PELO PRÓPRIO SERVIDOR*/

//Essa função faz um post de uma página HTML contendo a palavra _a_ no IP do servidor

void update_page(){
  if (esp8266.available()){
    //Acha o começo da resposta do TCP/IP para ver o Id de conexão
    if (esp8266.find("+IPD,")){
      delay(100);
      //Esse ID é a primeira coisa depois de +IPD, na resposta, porem como é uma string, precisamos transformar o numero
      //de ASCII para inteiro
      int connectionId = esp8266.read() - 48;


      //Define a página HTML a ser postada no endereço de IP do próprio servidor
      String webpage = "<head><meta http-equiv=""refresh"" content=""2"">";
      webpage += "</head>";
     
      webpage += "<p>";
      webpage += "Candidato 11:";
      webpage += String(contador11);
      webpage += "</p>";
      webpage += "<p>";
      webpage += "Candidato 22:";
      webpage += String(contador22);
      webpage += "</p>";
      webpage += "<p>";
      webpage += "Candidato 33:";
      webpage += String(contador33);
      webpage += "</p>";


      //Envia a define o que vai ser enviado para o IP do servidor (o comando AT+CIPSEND tem como parametros
      //A o ID de conexão e o tamanho da string a ser enviada
      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      cipSend += webpage.length();
      cipSend += "\r\n";
 
      sendData(cipSend, 2000, DEBUG);
      sendData(webpage, 2000, DEBUG);
      

    }
  }
  Serial.println("updated webapge");
}



/* FUNÇÃO DE CONFIGURAÇÃO INICIAL DO ESP8266 COMO SERVIDOR PARA MULTIPLAS CONEXÕES*/

void Setting_ESP(){  
  String flag_wifi;
  
  /*
  *inicialmente  se testa simplesmente se o ESP8266 está se comunicando com o arduino
  */
  
  Serial.println("configurando ESP8266");
  
  //Comando para que o ESP imprima o comando que foi enviado a ele e a resposta (echo)
  sendData("ATE1\r\n", 2000);
  /*
   * Agora conectamos o ESP a rede e o configuramos como servidor
   */
  //Muda o ESP para o modo 1 (modo Station, não Access Point)
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG);
  //Checa se o ESP esta conectado
  flag_wifi = sendData("AT+CWJAP?\r\n", 2000);

  if (flag_wifi[0] == 'N'){
    //Se ele nao estiver conectado a nenhum AP (access point) temos que conecta-lo
    Serial.println("Not connected to any AP. Trying to connect");
    //O comando AT utilizado é o AT+CWJAP="ssid","senha"
    sendData("AT+CWJAP=\"Felipe"",\"1234567890123\"\r\n", 2000, DEBUG);
  }
  
  //Quando o ESP Estiver conectado
  //Imprime o IP dele (servidor)
  Serial.println("Checking IP");
  sendData("AT+CIFSR\r\n", 1000, DEBUG);
  //E configura para multiplas conexões
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG);
  //Então incia o servidor na porta 80
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG);

  //Por fim desabilita a função que faz com que o ESP imprima o comando mandado para ele (disable echo)
  sendData("ATE0\r\n", 2000);
  Serial.println("End of setting up");
}



