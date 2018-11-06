void display_print(int i) {
  float temp = temperatura;
  switch(i) {  
    //menu principal
    case 0:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Vote para PRESIDENTE:");
      display.setCursor(0,8);
      display.println("Entre com 2 digitos");
      display.setCursor(0,16);
      display.println("Seguidos de '*' ou '#'");
      display.setCursor(0,24);
      display.println("Para cancelar ou confirmar");
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
  
    default:
    break;

  }
  
}

int memoriaLivre(unsigned int memPosition){
  int espacoLivre = readEeprom(memPosition);
  if (espacoLivre == 0xFFFF){
    espacoLivre = 0;
  }
  return espacoLivre;
}



