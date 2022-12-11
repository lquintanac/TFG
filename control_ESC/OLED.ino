void initOLED()
{
  Wire.begin(21,22);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS,false,false)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2);
  display.println(" RED BARON");
  display.display();
  delay(1000);
}
