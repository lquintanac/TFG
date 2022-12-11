#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pines I/O
  #define pinJX  4
  #define pinJY  39
  #define pinJB  17 // TX2
// Pines RF
  #define pinCE  1// VSPI: 15  HSPI: 1
  #define pinCSN 5//5 VSPI: 2   HSPI: 5
// Variables OLED
  #define OLED_ANCHO     128
  #define OLED_ALTO      64
  #define OLED_RESET     16   //RX2
  #define OLED_DIRECCION 0x3C


  SPIClass * hspi = NULL;

// Objeto radio
  RF24 radio(pinCE, pinCSN);
// Objeto oled
  Adafruit_SSD1306 oled(OLED_ANCHO, OLED_ALTO, &Wire, OLED_RESET);
// Handles para los nucleos
TaskHandle_t RF;        // Handle para core 0
TaskHandle_t Taskloop;  // Handle para core 1

double ant, cur;

struct Datos_Mando{   //MAXIMO 32 BYTES
  uint16_t JX;            //1
  uint16_t JY;            //1
  bool     JB;            //1
};

struct Datos_Dron{    //MAXIMO 32 BYTES
  float Tension;      //4
};

Datos_Mando datosm;
Datos_Dron  datosd;

const byte direccion[][6] = {"00001", "00002"};

int PaquetePerdido = 0;

#define FILTER_LEN  8
int ContAvgRead = 0;
uint64_t SumAvgRead = 0;
uint32_t AvgRead = 0;
int AN_Pot1_i = 0;
uint32_t AN_Pot1_Buffer[FILTER_LEN] = {0};

void setup(void)
{
  Serial.begin(115200);
    
  pinMode(pinJX, INPUT);
  pinMode(pinJY, INPUT);
  pinMode(pinJB, INPUT_PULLUP);
  Serial.println(getCpuFrequencyMhz());

  hspi = new SPIClass(HSPI);
  hspi->begin();

// Setup nRF24L01+
  while(radio.failureDetected){
      Serial.println("ERROR Hardware de radio");
      delay(1000);}
  Serial.println("Hardware de radio correcto");
  while(!radio.begin(hspi)){
      Serial.println("ERROR radio.begin");
      delay(1000);}
  Serial.println("EXITO radio.begin");
  
  radio.openWritingPipe(direccion[0]);    //
  radio.openReadingPipe(1, direccion[1]); //
  radio.setPALevel(RF24_PA_LOW, 1);       //
  radio.setChannel(120);                  // 2400MHz + 120 * 1MHz = 2520MHz
  radio.setDataRate(RF24_1MBPS);          //
  //radio.setAutoAck(false);              //
  
// Setup pantalla SSD1306
  while(!oled.begin(SSD1306_SWITCHCAPVCC, OLED_DIRECCION)){
      Serial.println("ERROR oled.begin");
      delay(1000);}
  Serial.println("EXITO oled.begin");
  oled.clearDisplay();

  Serial.print("setup() running on core ");
  Serial.println(xPortGetCoreID());

// Creacion de tareas de cada nucleo
  xTaskCreatePinnedToCore(
  Transmision,  // Funcion que implementa la tarea
  "RF",         // Nombre de la tarea
  4096,         // Stack size
  NULL,         // Parametro de entrada de la tarea
  1,            // Prioridad de la tarea
  &RF,          // Handle de la tarea
  0);           // Nucleo que ejecuta la tarea
  delay(500);
  
  xTaskCreatePinnedToCore(
  TaskLoop,     // Funcion que implementa la tarea
  "Taskloop",   // Nombre de la tarea
  4096,         // Stack size
  NULL,         // Parametro de entrada de la tarea
  1,            // Prioridad de la tarea
  &Taskloop,    // Handle de la tarea
  1);           // Nucleo que ejecuta la tarea
  delay(500);
}
 
void TaskLoop(void * pvParameters){
  delay(2);
  for(;;){
        delay(1);
    /*Serial.print("JX: ");
    Serial.print(datosm.JX);
    Serial.print("\tJY: ");
    Serial.print(datosm.JY);
    Serial.print("\tJB: ");
    Serial.print(datosm.JB);
    Serial.print("\tTension: ");
    Serial.print(datosd.Tension);
    Serial.print("\tTaskLoop: ");
    Serial.print(xPortGetCoreID());
    Serial.print("\tPaq Perdidos: ");
    Serial.println(PaquetePerdido);*/

    
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(0, 0);
    oled.print("JX:");
    oled.setCursor(0, 16);
    oled.print(datosm.JX);
    oled.setCursor(45, 0);
    oled.print("JY:");
    oled.setCursor(45, 16);
    oled.print(datosm.JY);
    oled.setCursor(90, 0);
    oled.print("JB:");
    oled.setCursor(90, 16);
    oled.print(datosm.JB);
    oled.setCursor(0, 32);
    oled.print("PL:");
    oled.setCursor(0, 48);
    oled.print(PaquetePerdido);
    oled.setCursor(45, 32);
    oled.print("V:");
    oled.setCursor(45, 48);
    oled.print(datosd.Tension);
    oled.display();      // Show initial text
    delay(10);

    
  }
}

void Transmision(void * pvParameters){
  delay(2);
  for(;;){

    radio.startListening();
    //delayMicroseconds(5000);
    delay(5);
    
    delayMicroseconds(1855);
    
    if (radio.available()) {
        radio.read(&datosd, sizeof(datosd));
      }

    else{
      resetDatosDron();
      PaquetePerdido++;
                                  Serial.print("PL ");

    }
    radio.stopListening();

    datosm.JX = map(analogReadAvg (pinJX), 0, 4095, 0, 255);
    datosm.JY = map(analogReadAvg (pinJY), 0, 4095, 0, 255);
    datosm.JB = digitalRead(pinJB);

    radio.write(&datosm, sizeof(datosm));
                            cur = micros() - ant;
                            Serial.println(cur);
                            ant = micros();     // 38950
  }
}

void resetDatosDron(){
  datosd.Tension = -1;
}

/*uint16_t analogReadAvg (const int PIN){
  ContAvgRead = 0;
  SumAvgRead = 0;
  
  AN_Pot1_Buffer[AN_Pot1_i++] = analogRead(PIN);
  if(AN_Pot1_i == FILTER_LEN)
  {
    AN_Pot1_i = 0;
  }
  for(ContAvgRead=0; ContAvgRead<FILTER_LEN; ContAvgRead++)
  {
    SumAvgRead += AN_Pot1_Buffer[ContAvgRead];
  }
  return (SumAvgRead/FILTER_LEN);
}*/

uint16_t analogReadAvg (const int PIN){
  SumAvgRead = 0;
  for(ContAvgRead = 0; ContAvgRead < FILTER_LEN; ContAvgRead++){
    SumAvgRead += analogRead (PIN);
  }
  AvgRead = SumAvgRead / FILTER_LEN;
  return AvgRead;
}

void loop(){
  
}
