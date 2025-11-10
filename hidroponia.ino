// Control de bomba con Adafruit Motor Shield v1
#include <Wire.h>
#include <AFMotor_R4.h>

const uint8_t PUMP1_MOTOR_PORT = 1;                        // Puerto M1 del shield
const uint8_t PUMP2_MOTOR_PORT = 2;                        // Puerto M2 del shield
const uint8_t PUMP_SPEED = 255;                            // Velocidad máxima (0-255)
const unsigned long MOTOR_RUN_DURATION = 5UL * 60UL * 1000UL; // 5 minutos en milisegundos
const unsigned long WAIT_DURATION = 50UL * 60UL * 1000UL;     // 50 minutos en milisegundos

AF_DCMotor bomba1(PUMP1_MOTOR_PORT);
AF_DCMotor bomba2(PUMP2_MOTOR_PORT);

enum CycleStage {
  STAGE_PUMP1,
  STAGE_PUMP2,
  STAGE_WAIT
};

CycleStage currentStage = STAGE_PUMP1;
unsigned long stageStartMillis = 0;

void enterStage(CycleStage newStage) {
  currentStage = newStage;
  stageStartMillis = millis();

  switch (newStage) {
    case STAGE_PUMP1:
      bomba1.setSpeed(PUMP_SPEED);
      bomba1.run(FORWARD);
      Serial.println(F("Bomba M1 encendida: iniciando riego."));
      break;
    case STAGE_PUMP2:
      bomba2.setSpeed(PUMP_SPEED);
      bomba2.run(FORWARD);
      Serial.println(F("Bomba M2 encendida: continuando riego."));
      break;
    case STAGE_WAIT:
      Serial.println(F("Periodo de espera iniciado."));
      break;
  }
}

void setup() {
  Serial.begin(9600);
#if defined(USBCON) || defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_RP2040)
  while (!Serial) {
    delay(10);
  }
#endif

  Serial.println(F("Configurando Adafruit Motor Shield v1..."));
  bomba1.setSpeed(PUMP_SPEED);
  bomba1.run(RELEASE);
  bomba2.setSpeed(PUMP_SPEED);
  bomba2.run(RELEASE);
  Serial.println(F("Bombas apagadas (modo espera)."));

  enterStage(STAGE_PUMP1);
}

void loop() {
  unsigned long currentMillis = millis();

  switch (currentStage) {
    case STAGE_PUMP1:
      if (currentMillis - stageStartMillis >= MOTOR_RUN_DURATION) {
        bomba1.run(RELEASE);
        Serial.println(F("Bomba M1 apagada tras finalizar el riego."));
        enterStage(STAGE_PUMP2);
      }
      break;
    case STAGE_PUMP2:
      if (currentMillis - stageStartMillis >= MOTOR_RUN_DURATION) {
        bomba2.run(RELEASE);
        Serial.println(F("Bomba M2 apagada tras finalizar el riego."));
        enterStage(STAGE_WAIT);
      }
      break;
    case STAGE_WAIT:
      if (currentMillis - stageStartMillis >= WAIT_DURATION) {
        enterStage(STAGE_PUMP1);
      }
      break;
  }
}
