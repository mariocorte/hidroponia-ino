const uint8_t PUMP_PIN = 2;
const unsigned long PUMP_INTERVAL = 10UL * 60UL * 60UL * 1000UL; // 10 hours in milliseconds
const unsigned long PUMP_DURATION = 3UL * 60UL * 1000UL;        // 3 minutes in milliseconds

unsigned long lastPumpStart = 0;
bool pumpRunning = false;
bool firstRun = true;

void setup() {
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
}

void loop() {
  unsigned long currentMillis = millis();

  if (pumpRunning) {
    if (currentMillis - lastPumpStart >= PUMP_DURATION) {
      digitalWrite(PUMP_PIN, LOW);
      pumpRunning = false;
    }
  } else {
    if (firstRun || currentMillis - lastPumpStart >= PUMP_INTERVAL) {
      digitalWrite(PUMP_PIN, HIGH);
      lastPumpStart = currentMillis;
      pumpRunning = true;
      firstRun = false;
    }
  }
}
