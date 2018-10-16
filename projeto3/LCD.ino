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
    break;
  
    case 3:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Dados gravados");
      display.println(memoriaLivre(2046));
      //display.setCursor(0,16);
      display.println("Medicoes disponiveis: ");
      display.println(1024 - memoriaLivre(2046));
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

int memoriaLivre(unsigned int memPosition){
  int espacoLivre = readEeprom(memPosition);
  if (espacoLivre == 0xFFFF){
    espacoLivre = 0;
  }
  return espacoLivre;
}

