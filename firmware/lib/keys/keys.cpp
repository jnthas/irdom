
#include "keys.h"

namespace Keys {
  
  uint16_t keyRead = 0;

  unsigned long currentMillis = 0;

  void setup() {
    pinMode(KEYS_PIN, INPUT);

  }

  void loop(KeyEvent keyEvent) {

    if (millis()-currentMillis > 100) {

      keyRead = analogRead(KEYS_PIN);

      if (keyRead >= BUTTON_A_THRESHOLD) {
        keyEvent(Keys::BUTTON_A, Keys::PRESSED);
              
      } else if (keyRead >= BUTTON_B_THRESHOLD && keyRead < BUTTON_A_THRESHOLD) {
        keyEvent(Keys::BUTTON_B, Keys::PRESSED);

      } else if (keyRead >= BUTTON_C_THRESHOLD && keyRead < BUTTON_B_THRESHOLD) {
        keyEvent(Keys::BUTTON_C, Keys::PRESSED);

      }

      currentMillis = millis();

    }
  }
  
};
