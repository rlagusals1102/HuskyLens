#include "HUSKYLENS.h"
#include "Wire.h"

const int LED_PINS[] = { 5, 6, 7 };
const int LED_lengths = sizeof(LED_PINS) / sizeof(LED_PINS[0]);
bool ledStates[LED_lengths] = { false };
HUSKYLENS huskylens;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  for (int i = 0; i < LED_lengths; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }

  while (!huskylens.begin(Wire)) {
    Serial.println("Begin failed");
    delay(100);
  }
}

void loop() {
  processData();
}

bool processData() {
  memset(ledStates, 0, sizeof(ledStates));
  bool detected = false;

  if (huskylens.request()) {
    while (huskylens.available()) {
      HUSKYLENSResult result = huskylens.read();
      if (result.command == COMMAND_RETURN_BLOCK && result.ID > 0 && result.ID <= LED_lengths) {
        Serial.println(result.ID);
        ledStates[result.ID - 1] = true;
        detected = true;
      } else {
        Serial.println("Object unknown or invalid ID");
      }
    }
    updateLEDs();
    return true;
  }

  if (!detected) {
    turnOffAllLEDs();
  }

  return false;
}

void updateLEDs() {
  for (int i = 0; i < LED_lengths; i++) {
    digitalWrite(LED_PINS[i], ledStates[i] ? HIGH : LOW);
  }
}

void turnOffAllLEDs() {
  for (int i = 0; i < LED_lengths; i++) {
    digitalWrite(LED_PINS[i], LOW);
  }
}