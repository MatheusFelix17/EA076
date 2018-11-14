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
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Numero: 11");
      display.setCursor(0,8);
      display.println("Nome: Tony Stark");
      display.setCursor(0,16);
      display.println("Partido: PSI");
      display.setCursor(0,24);
      display.println("# confirmar");
      display.setCursor(0,32);
      display.println("* cancelar");
      display.display();
    break;

    case 17:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Numero: 17");
      display.setCursor(0,8);
      display.println("Nome: Gustavo");
      display.setCursor(0,16);
      display.println("Partido: NSL");
      display.setCursor(0,24);
      display.println("# confirmar");
      display.setCursor(0,32);
      display.println("* cancelar");
      display.display();
    break;

    case 22:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Numero: 22");
      display.setCursor(0,8);
      display.println("Nome: Wayne");
      display.setCursor(0,16);
      display.println("Partido: PCG");
      display.setCursor(0,24);
      display.println("# confirmar");
      display.setCursor(0,32);
      display.println("* cancelar");
      display.display();
    break;

    case 33:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Numero: 33");
      display.setCursor(0,8);
      display.println("Nome: Goku");
      display.setCursor(0,16);
      display.println("Partido: UN7");
      display.setCursor(0,24);
      display.println("# confirmar");
      display.setCursor(0,32);
      display.println("* cancelar");
      display.display();
    break;

    case 44:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Numero: 44");
      display.setCursor(0,8);
      display.println("Nome: Vegeta");
      display.setCursor(0,16);
      display.println("Partido: VGT");
      display.setCursor(0,24);
      display.println("# confirmar");
      display.setCursor(0,32);
      display.println("* cancelar");
      display.display();
    break;

    case 55:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Numero: 55");
      display.setCursor(0,8);
      display.println("Nome: Beerus");
      display.setCursor(0,16);
      display.println("Partido: GOD");
      display.setCursor(0,24);
      display.println("# confirmar");
      display.setCursor(0,32);
      display.println("* cancelar");
      display.display();
    break;

    case 66:
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Numero: 66");
      display.setCursor(0,8);
      display.println("Nome:Palpatine");
      display.setCursor(0,16);
      display.println("Partido: EMP");
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

int memoriaLivre(unsigned int memPosition){
  int espacoLivre = readEeprom(memPosition);
  if (espacoLivre == 0xFFFF){
    espacoLivre = 0;
  }
  return espacoLivre;
}
