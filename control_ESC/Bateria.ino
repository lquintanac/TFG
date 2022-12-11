void LeerBat(){
  String mensaje;
  batRaw = 0;
  for(int i = 0; i < batMuestras; i++){
    batRaw += analogRead(pinBAT);
  }
    batRaw /= batMuestras;
    batTension = batRaw / 263.1;
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(1);
    display.println("Bateria");
    display.setCursor(0,16);
    display.setTextSize(2);
    display.println(batTension);
    display.display();

    mensaje = String(batTension) + "\n";
    
    appendFile(SD, "/bateria.txt", mensaje);
}
