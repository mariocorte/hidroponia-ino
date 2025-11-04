// Control de bomba con Adafruit Motor Shield v1
#include <Wire.h>
#include <AFMotor.h>

const uint8_t PUMP_MOTOR_PORT = 1;                              // Puerto M1 del shield
const uint8_t PUMP_SPEED = 255;                                 // Velocidad máxima (0-255)
const unsigned long PUMP_INTERVAL = 10UL * 60UL * 60UL * 1000UL; // 10 horas en milisegundos
const unsigned long PUMP_DURATION = 3UL * 60UL * 1000UL;        // 3 minutos en milisegundos

AF_DCMotor bomba(PUMP_MOTOR_PORT);

unsigned long lastPumpStart = 0;
bool pumpRunning = false;
bool firstRun = true;

void setup() {
  Serial.begin(9600);
#if defined(USBCON) || defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_RP2040)
  while (!Serial) {
    delay(10);
  }
#endif

  Serial.println(F("Configurando Adafruit Motor Shield v1..."));
  bomba.setSpeed(PUMP_SPEED);
  bomba.run(RELEASE);
  Serial.println(F("Bomba apagada (modo espera)."));
}

void loop() {
  unsigned long currentMillis = millis();

  if (pumpRunning) {
    if (currentMillis - lastPumpStart >= PUMP_DURATION) {
      bomba.run(RELEASE);
      pumpRunning = false;
      Serial.println(F("Bomba apagada tras finalizar el riego."));
    }
  } else {
    if (firstRun || currentMillis - lastPumpStart >= PUMP_INTERVAL) {
      bomba.setSpeed(PUMP_SPEED);
      bomba.run(FORWARD);
      lastPumpStart = currentMillis;
      pumpRunning = true;
      firstRun = false;
      Serial.println(F("Bomba encendida: iniciando riego."));
    }
  }
}
