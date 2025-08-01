#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2      // DS18B20 on digital pin 2
#define HEATER_PIN 3        // LED simulating heater
#define BUZZER_PIN 4        // Buzzer or alert LED


// Internship Assingment by Tirthankar karmakar

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

enum HeaterState { IDLE, HEATING, STABILIZING, TARGET_REACHED, OVERHEAT };
HeaterState currentState = IDLE;

float currentTemp = 0.0;
const float TARGET_TEMP = 35.0;             // Desired heating temp
const float MAX_TEMP = 50.0;                // Overheat threshold (increased from 45)
const float STABILIZING_THRESHOLD = 1.0;

unsigned long lastReadTime = 0;
const unsigned long interval = 1000; // 1s interval

void setup() {
  Serial.begin(9600);
  sensors.begin();
  pinMode(HEATER_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(HEATER_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  Serial.println("System Initialized.");
}

void loop() {
  unsigned long now = millis();
  if (now - lastReadTime >= interval) {
    lastReadTime = now;
    sensors.requestTemperatures();
    currentTemp = sensors.getTempCByIndex(0);

    // Skip if invalid temperature
    if (currentTemp == -127 || currentTemp == -196) {
      Serial.println("⚠️ Sensor error or disconnected.");
      return;
    }

    Serial.print("Temperature: ");
    Serial.print(currentTemp);
    Serial.print("°C | State: ");

    // 🔥 Global Overheat Check
    if (currentTemp > MAX_TEMP) {
      currentState = OVERHEAT;
      digitalWrite(HEATER_PIN, LOW);
      digitalWrite(BUZZER_PIN, HIGH);
      Serial.println("⚠️ OVERHEAT! Immediate shutdown.");
      return; // Skip rest of the state logic
    }

    switchState(currentTemp);
  }
}

void switchState(float temp) {
  switch (currentState) {
    case IDLE:
      digitalWrite(HEATER_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
      if (temp < TARGET_TEMP - 2) {
        currentState = HEATING;
        Serial.println("IDLE → HEATING");
      } else {
        Serial.println("IDLE");
      }
      break;

    case HEATING:
      digitalWrite(HEATER_PIN, HIGH);
      digitalWrite(BUZZER_PIN, LOW);
      if (temp >= TARGET_TEMP) {
        currentState = STABILIZING;
        Serial.println("HEATING → STABILIZING");
      } else {
        Serial.println("HEATING");
      }
      break;

    case STABILIZING:
      digitalWrite(HEATER_PIN, LOW);  // Allow temp to settle
      if (abs(temp - TARGET_TEMP) <= STABILIZING_THRESHOLD) {
        currentState = TARGET_REACHED;
        Serial.println("STABILIZING → TARGET_REACHED");
      } else if (temp < TARGET_TEMP - 2) {
        currentState = HEATING;
        Serial.println("STABILIZING → HEATING");
      } else {
        Serial.println("STABILIZING");
      }
      break;

    case TARGET_REACHED:
      digitalWrite(HEATER_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
      Serial.println("TARGET_REACHED");
      if (temp > MAX_TEMP) {
        currentState = OVERHEAT;
        Serial.println("TARGET_REACHED → OVERHEAT");
      }
      break;

    case OVERHEAT:
      digitalWrite(HEATER_PIN, LOW);
      digitalWrite(BUZZER_PIN, HIGH);
      Serial.println("⚠️ OVERHEAT! Heater OFF. Buzzer ON.");
      

      // ✅ Auto-recovery logic
  if (temp < MAX_TEMP - 5) {
    currentState = IDLE;
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println("Recovered from OVERHEAT → IDLE");
  }
  break;
  }
}
