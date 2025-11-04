// Control de bomba con Adafruit Motor Shield V2
#include <Wire.h>
#include <Adafruit_MotorShield.h>

const uint8_t PUMP_MOTOR_PORT = 1;                              // Puerto M1 del shield
const uint8_t PUMP_SPEED = 255;                                 // Velocidad máxima (0-255)
const unsigned long PUMP_INTERVAL = 10UL * 60UL * 60UL * 1000UL; // 10 horas en milisegundos
const unsigned long PUMP_DURATION = 3UL * 60UL * 1000UL;        // 3 minutos en milisegundos

Adafruit_MotorShield motorShield = Adafruit_MotorShield();
Adafruit_DCMotor *pumpMotor = motorShield.getMotor(PUMP_MOTOR_PORT);

unsigned long lastPumpStart = 0;
bool pumpRunning = false;
bool firstRun = true;

void setup() {
  if (!motorShield.begin()) {
    // Si el shield no se inicializa, permaneceremos sin activar la bomba
    while (true) {
      delay(1000);
    }
  }

  pumpMotor->setSpeed(PUMP_SPEED);
  pumpMotor->run(RELEASE);
}

void loop() {
  unsigned long currentMillis = millis();

  if (pumpRunning) {
    if (currentMillis - lastPumpStart >= PUMP_DURATION) {
      pumpMotor->run(RELEASE);
      pumpRunning = false;
    }
  } else {
    if (firstRun || currentMillis - lastPumpStart >= PUMP_INTERVAL) {
      pumpMotor->setSpeed(PUMP_SPEED);
      pumpMotor->run(FORWARD);
      lastPumpStart = currentMillis;
      pumpRunning = true;
      firstRun = false;
    }
  }
}
