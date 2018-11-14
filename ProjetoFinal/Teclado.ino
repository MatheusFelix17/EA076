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
