void display_print(int i) {

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

    case 11:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Numero: 11");
      display.setCursor(0,8);
      display.println("Nome: Tony Stark");
      display.setCursor(0,16);
      display.println("Partido: PSI");
      display.setCursor(0,24);
      display.println("Pressione # para confirmar");
      display.setCursor(0,32);
      display.println("Pressione * para cancelar");
      display.display();
    break;

    case 17:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Numero: 17");
      display.setCursor(0,8);
      display.println("Nome: Gustavo Simonini");
      display.setCursor(0,16);
      display.println("Partido: NSL");
      display.setCursor(0,24);
      display.println("Pressione # para confirmar");
      display.setCursor(0,32);
      display.println("Pressione * para cancelar");
      display.display();
    break;

    case 22:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Numero: 22");
      display.setCursor(0,8);
      display.println("Nome: Bruce Wayne");
      display.setCursor(0,16);
      display.println("Partido: PCG");
      display.setCursor(0,24);
      display.println("Pressione # para confirmar");
      display.setCursor(0,32);
      display.println("Pressione * para cancelar");
      display.display();
    break;

    case 33:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Numero: 33");
      display.setCursor(0,8);
      display.println("Nome: ");
      display.setCursor(0,16);
      display.println("Partido: ");
      display.setCursor(0,24);
      display.println("Pressione # para confirmar");
      display.setCursor(0,32);
      display.println("Pressione * para cancelar");
      display.display();
    break;

    case 44:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Numero: 44");
      display.setCursor(0,8);
      display.println("Nome: ");
      display.setCursor(0,16);
      display.println("Partido: ");
      display.setCursor(0,24);
      display.println("Pressione # para confirmar");
      display.setCursor(0,32);
      display.println("Pressione * para cancelar");
      display.display();
    break;

    case 55:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Numero: 55");
      display.setCursor(0,8);
      display.println("Nome: ");
      display.setCursor(0,16);
      display.println("Partido: ");
      display.setCursor(0,24);
      display.println("Pressione # para confirmar");
      display.setCursor(0,32);
      display.println("Pressione * para cancelar");
      display.display();
    break;

    case 66:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Numero: 66");
      display.setCursor(0,8);
      display.println("Nome: Sheev Palpatine");
      display.setCursor(0,16);
      display.println("Partido: EMP");
      display.setCursor(0,24);
      display.println("Pressione # para confirmar");
      display.setCursor(0,32);
      display.println("Pressione * para cancelar");
      display.display();
    break;

    case 100:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Voto registrado com sucesso!");
      display.display();
    break;
    
    //elses: voto NULO
    default:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println(i);
      display.setCursor(0,8);
      display.println("NUMERO ERRADO");
      display.setCursor(4,16);    //tentar centralizar, nao sei se vai bugar igual quando aumentamos text size
      display.println("VOTO NULO");
      display.setCursor(0,24);
      display.println("Pressione # para confirmar");
      display.setCursor(0,32);
      display.println("Pressione * para cancelar");
      display.display();
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

