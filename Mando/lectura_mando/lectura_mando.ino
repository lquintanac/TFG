#define pin_canal1 12
#define pin_canal2 14
#define pin_canal3 35
#define pin_canal4 34

long loop_timer, tiempo_ejecucion;
float wPitch, wRoll, wYaw, pulsoPotencia;

// MANDO POTENCIA
const int PulsoMaxPotencia = 2000;
const int PulsoMinPotencia = 1000;
const float tMaxPotencia = 2;  // &lt; – Si teneis la entrada Throttle invertida sustituid este valor
const float tMinPotencia = 1;  // &lt; – por este y viceversa

const float tMax = 2;
const float tMin = 1;

// MANDO PITCH
const int wMaxPitch = -30; // &lt; – Si teneis la entrada Pitch invertida sustituid este valor
const int wMinPitch = 30;  // &lt; – por este y viceversa

// MANDO ROLL
const int wMaxRoll = 30;  // &lt; – Si teneis la entrada Roll invertida sustituid este valor
const int wMinRoll = -30; // &lt; – por este y viceversa

// MANDO YAW
const int wMaxYaw = 30;  // &lt; – Si teneis la entrada Yaw invertida sustituid este valor
const int wMinYaw = -30; // &lt; – por este y viceversa

uint32_t canal3,canal1,canal2,canal4 = 0;

volatile long contPotenciaInit; // LEER MANDO RC POTENCIA
volatile int PulsoPotencia;
void INTpotencia() {
  if (digitalRead(pin_canal3) == HIGH) contPotenciaInit = micros();
  if (digitalRead(pin_canal3) == LOW)PulsoPotencia = micros() - contPotenciaInit;
}

volatile long contPitchInit; // LEER MANDO RC PITCH
volatile int PulsoPitch;
void INTpitch() {
  if (digitalRead(pin_canal2) == HIGH) contPitchInit = micros();
  if (digitalRead(pin_canal2) == LOW) PulsoPitch = micros() - contPitchInit;
}

volatile long contRollInit; // LEER MANDO RC ROLL
volatile int PulsoRoll;
void INTroll() {
  if (digitalRead(pin_canal1) == HIGH) contRollInit = micros();
  if (digitalRead(pin_canal1) == LOW) PulsoRoll = micros() - contRollInit;
}


volatile long contYawInit; // LEER MANDO RC YAW
volatile int PulsoYaw;
void INTyaw() {
  if (digitalRead(pin_canal4) == HIGH) contYawInit = micros();
  if (digitalRead(pin_canal4) == LOW) PulsoYaw = micros() - contYawInit;
}

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);

  pinMode(pin_canal3, INPUT_PULLUP);                  // POTENCIA
  attachInterrupt(pin_canal3, INTpotencia, CHANGE);
   pinMode(pin_canal4, INPUT_PULLUP);                  // YAW
  attachInterrupt(pin_canal4, INTyaw, CHANGE);
  pinMode(pin_canal2, INPUT_PULLUP);                 // PITCH
  attachInterrupt(pin_canal2, INTpitch, CHANGE);
  pinMode(pin_canal1, INPUT_PULLUP);                  // ROLL
  attachInterrupt(pin_canal1, INTroll, CHANGE);

  
  
}

void loop() {
  // put your main code here, to run repeatedly:

    // =============== Ecuaciones de procesamiento
  wPitch =   ((wMinPitch - wMaxPitch) / (tMax - tMin)) * ((PulsoPitch ) / 1000.00 - tMin) + wMaxPitch;
  wRoll =   ((wMaxRoll - wMinRoll) / (tMax - tMin)) * ((PulsoRoll) / 1000.00 - tMin) + wMinRoll;
  wYaw =  ((wMinYaw - wMaxYaw) / (tMax - tMin)) * ((PulsoYaw) / 1000.00 - tMin) + wMaxYaw;
  pulsoPotencia =  ((PulsoMaxPotencia - PulsoMinPotencia) / (tMinPotencia)) * ((PulsoPotencia) / 1000.00 - tMaxPotencia) + PulsoMinPotencia;
  // =============== Ecuaciones de procesamiento


  Serial.print(pulsoPotencia);
  Serial.print(",");
  Serial.print(wPitch);
  Serial.print(",");
  Serial.print(wRoll);
  Serial.print(",");
  Serial.println(wYaw);
  


  

  
}


void MandarDatosSimulink()
{
  byte paquete[6];
  paquete[0] = 'T';
  paquete[1] = pulsoPotencia;
  paquete[2] = wPitch;
  paquete[3] = wRoll;
  paquete[4] = wYaw;
  paquete[5] = '\n';

  Serial.write(paquete,6);
  
}
