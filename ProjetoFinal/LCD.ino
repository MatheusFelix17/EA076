void display_print(int i) {

  switch(i) {  
    //menu principal
    case 0:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Vote para");
      display.setCursor(0,8);
      display.println("PRESIDENTE:");
      display.setCursor(0,16);
      display.println("2 digitos +");
      display.setCursor(0,24);
      display.println("'*' cancelar");
      display.setCursor(0,32);
      display.println("'#' confirmar");
      display.display();
    break;

    case 11:
    case 22:
    case 33:
      if (i == 11){
        contador11++;
      }
      if (i == 22){
        contador22++;
      }
      if (i == 33){
        contador33++;
      }
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Numero de candidato");
      display.println(i);
      display.setCursor(0,24);
      display.println("# confirmar");
      display.setCursor(0,32);
      display.println("* cancelar");
      display.display();
    break;
    
    case 100:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Voto");
      display.setCursor(0,8);
      display.println(" Registrado!");
      display.display();
    break;
    
    //elses: voto NULO
    default:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println(i);
      display.setCursor(0,8);
      display.println("NUMERO ERRADO");
      display.setCursor(4,16);    //tentar centralizar, nao sei se vai bugar igual quando aumentamos text size 14 caracteres parece quase ocupar tudo
      display.println("VOTO NULO");
      display.setCursor(0,24);
      display.println("# confirmar");
      display.setCursor(0,32);
      display.println("* cancelar");
      display.display();
    break;

  }
  
}
