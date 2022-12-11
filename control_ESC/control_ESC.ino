#include "BluetoothSerial.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define SD_CS 13
#define SDSPEED 1000000
SPIClass spiSD(HSPI);


#define pinCH1 36 
#define pinCH2 39 
#define pinCH3 34 
#define pinCH4 35 
#define pinCH5 12 
#define pinCH6 0 

#define pinM1 1 // connected to ESC control wire  19
#define pinM2 3 // connected to ESC control wire  23
#define pinM3 19 // connected to ESC control wire  1
#define pinM4 23 // connected to ESC control wire  3

#define pinBAT 25 // not defaulted properly for ESP32s/you must define it

// Nucleos
TaskHandle_t HCore0;  // Handle para core 0
TaskHandle_t HCore1;  // Handle para core 1

// ESC 
#define MIN_SPEED 200
#define MAX_SPEED 410
int seleccion_motor = 5; //0 apagados todos, 1 seleccion motor1, ... ,5 seleccion todos los motores

// BT
BluetoothSerial SerialBT;

// OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// MPU
Adafruit_MPU6050 mpu;

// MANDO POTENCIA (CH3)
float pulsoPotencia;
const int PulsoMaxPotencia = 2000;
const int PulsoMinPotencia = 1000;
const float tMaxPotencia = 2;  // &lt; – Si teneis la entrada Throttle invertida sustituid este valor
const float tMinPotencia = 1;  // &lt; – por este y viceversa

const float tMax = 2;
const float tMin = 1;

volatile long contPotenciaInit; // LEER MANDO RC POTENCIA

// PWM
const int freq = 50;
const int resolucion = 12;
const int canal_pwm_M1 = 0;
const int canal_pwm_M2 = 1;
const int canal_pwm_M3 = 2;
const int canal_pwm_M4 = 4;

uint16_t pwmM1 = MIN_SPEED;
uint16_t pwmM2 = MIN_SPEED;
uint16_t pwmM3 = MIN_SPEED;
uint16_t pwmM4 = MIN_SPEED;

// BATERIA
unsigned int tiempo_anterior;
int batRaw;
float batTension;
const int batMuestras = 100;


// Interrupciones
volatile int PulsoPotencia;
void INTpotencia() {
  if (digitalRead(pinCH3) == HIGH) contPotenciaInit = micros();
  if (digitalRead(pinCH3) == LOW)PulsoPotencia = micros() - contPotenciaInit;
}

void setup() {
  initOLED();

  if(SerialBT.begin("DRONE")){
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(0);
    display.println("BT INICIADO");
    display.display();
    delay(1000);
  }
  else{
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(0);
    display.println("BT FAIL");
    display.display();
    delay(1000);
  }
  
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(0);
  display.println("BT INICIADO");
  display.display();
  delay(1000);

  MPUsetup();
  
  pinMode(pinM1, OUTPUT);
  pinMode(pinM2, OUTPUT);
  pinMode(pinM3, OUTPUT);
  pinMode(pinM4, OUTPUT);
  pinMode(pinBAT, INPUT);
  pinMode(pinCH3, INPUT_PULLUP);                  // POTENCIA
  attachInterrupt(pinCH3, INTpotencia, CHANGE);
  delay(1000);  
  
  ledcSetup(canal_pwm_M1, freq, resolucion);
  ledcSetup(canal_pwm_M2, freq, resolucion);
  ledcSetup(canal_pwm_M3, freq, resolucion);
  ledcSetup(canal_pwm_M4, freq, resolucion);
  
  ledcAttachPin(pinM1, canal_pwm_M1);
  ledcAttachPin(pinM2, canal_pwm_M2);
  ledcAttachPin(pinM3, canal_pwm_M3);
  ledcAttachPin(pinM4, canal_pwm_M4);
  
  ledcWrite(canal_pwm_M1, pwmM1);
  ledcWrite(canal_pwm_M2, pwmM2);
  ledcWrite(canal_pwm_M3, pwmM3);
  ledcWrite(canal_pwm_M4, pwmM4);
  delay(5000); // Wait a while

  
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("MOTORES ARMADOS");
  display.display();

  SerialBT.println("MOTORES ARMADOS");
  initSD();

  writeFile(SD, "/bateria.txt", "Tension\n");
  
  // Creacion de tareas de cada nucleo
  xTaskCreatePinnedToCore(
  Core0,    // Funcion que implementa la tarea
  "Core0",  // Nombre de la tarea
  4096,     // Stack size
  NULL,     // Parametro de entrada de la tarea
  1,        // Prioridad de la tarea
  &HCore0,  // Handle de la tarea
  0);       // Nucleo que ejecuta la tarea
  delay(500);
  
  xTaskCreatePinnedToCore(
  Core1,    // Funcion que implementa la tarea
  "Core1",  // Nombre de la tarea
  4096,     // Stack size
  NULL,     // Parametro de entrada de la tarea
  1,        // Prioridad de la tarea
  &HCore1,  // Handle de la tarea
  1);       // Nucleo que ejecuta la tarea
  delay(500);

  Serial.println("EXITO setup()");
} // speed will now jump to pot setting

void Core0(void * pvParameters){
  delay(2);
  for(;;){
    delay(1);
    pulsoPotencia =  ((PulsoMaxPotencia - PulsoMinPotencia) / (tMinPotencia)) * ((PulsoPotencia) / 1000.00 - tMaxPotencia) + PulsoMinPotencia;
  
    if(seleccion_motor == 0)
    {
      pwmM1 = pwmM2 = pwmM3 = pwmM4 = MIN_SPEED;
    }
    else if(seleccion_motor == 1)
    {
      pwmM1 = map(pulsoPotencia,0.0,1000.0,MIN_SPEED,MAX_SPEED);
      pwmM2 = pwmM3 = pwmM4 = MIN_SPEED;
    }
    else if(seleccion_motor == 2)
    {
      pwmM2 = map(pulsoPotencia,0.0,1000.0,MIN_SPEED,MAX_SPEED);
      pwmM1 = pwmM3 = pwmM4 = MIN_SPEED;
    }
    else if(seleccion_motor == 3)
    {
      pwmM3 = map(pulsoPotencia,0.0,1000.0,MIN_SPEED,MAX_SPEED);
      pwmM1 = pwmM2 = pwmM4 = MIN_SPEED;
    }
    else if(seleccion_motor == 4)
    {
      pwmM4 = map(pulsoPotencia,0.0,1000.0,MIN_SPEED,MAX_SPEED);
      pwmM1 = pwmM2 = pwmM3 = MIN_SPEED;
    }
    else if(seleccion_motor == 5)
    {
      pwmM1 = pwmM2 = pwmM3 = pwmM4 = map(pulsoPotencia,0.0,1000.0,MIN_SPEED,MAX_SPEED);
    }
   
    ledcWrite(canal_pwm_M1, pwmM1);
    ledcWrite(canal_pwm_M2, pwmM2);
    ledcWrite(canal_pwm_M3, pwmM3);
    ledcWrite(canal_pwm_M4, pwmM4);
  }
}
void Core1(void * pvParameters){
  delay(2);
  for(;;){
    delay(1);
    if(SerialBT.available())
    {
      char cmd = SerialBT.read();
  
      if(cmd == 'm' || cmd == 'M')
      {
        seleccion_motor = SerialBT.parseInt();
        SerialBT.print("Seleccion Motor: ");
        SerialBT.println(seleccion_motor);
      }
      else if(cmd == 'b' || cmd == 'B')
      {
        
        readFile(SD, "/bateria.txt");
        
      }
    }
    LeerBat();

  MPUloop();

  
  }
}
void loop() {
}
