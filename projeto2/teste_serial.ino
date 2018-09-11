byte lido;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
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
      Serial.println("Erro: tamanho maximo do comando excedido!");
    }
    else {
      Serial.print("tamanho do comando válido, que é: ");
      Serial.println(content);

      if (content == "VENT*") {
        Serial.println("OK VENT");
      }

      else if (content == "EXAUST*") {
        Serial.println("OK EXAUST");
      }
    
      else if (content == "PARA*") {
        Serial.println("OK PARA");
      }

      else if (content == "RETVEL*") {
        Serial.print("VEL");
        Serial.print(" X ");  //numero
        Serial.println("RPM");
      }

      else if (content.substring(0,3) == "VEL") {
        
        //vamos verificar se os próximos digitos de VEL
        String resto = content.substring(4,8);
        if (resto == "") {
          Serial.println("ERRO: PARÂMETRO AUSENTE");
        }

        else {
          if (resto[3] == '*') {
            Serial.print("Substring: ");
            Serial.println(resto);
            if(isDigit(resto[0]) && isDigit(resto[1]) && isDigit(resto[2])) {
              Serial.println("Os 3 valores são numeros");
              int num = resto.toInt();  
              Serial.print("Numero convertido = ");
              Serial.println(num);   
              if (num >= 0 && num <= 100) {
                Serial.println("Comando da velocidade OK!");    
                String resposta = "OK VEL ";
                resposta.concat(num);
                resposta.concat('%');   
                Serial.println(resposta);
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
