#include "display7.h"

namespace Display7 {
  bool currentDigit1 = true;

  unsigned long currentMillisDisplay = 0;
 
  uint8_t displayBuffer[2] = {0,0};

    
  void setup ()
  {
    // Setup pins as Outputs
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);

    pinMode(digit1, OUTPUT);
    pinMode(digit2, OUTPUT);
  }

  void loop()
  {
    refreshDisplay();
  }


  void writeDisplay(uint8_t a, uint8_t b) {
    displayBuffer[0] = a;
    displayBuffer[1] = b;

  }

  void refreshDisplay() {

    if ((millis() - currentMillisDisplay) >= 15) {
      
      digitalWrite(latchPin, LOW);

      if (currentDigit1) {
        digitalWrite(digit1, LOW);
        digitalWrite(digit2, HIGH);
        shiftOut(dataPin, clockPin, LSBFIRST, displayBuffer[0]);
      } else {
        digitalWrite(digit1, HIGH);
        digitalWrite(digit2, LOW);
        shiftOut(dataPin, clockPin, LSBFIRST, displayBuffer[1]);
      }

      digitalWrite(latchPin, HIGH);

      currentDigit1 = !currentDigit1;
      currentMillisDisplay = millis();

    }
  }



  uint8_t find_char(char digit) {
      for (uint8_t b = 0; b<CHAR_ARR_SIZE; b++) 
      {
          if (DisplayChars[b][1] == digit)
              return Display7::DisplayChars[b][0];
      }

      return Display7::DisplayChars[0][0];
  }

  void Display_setDigit(char digit, uint8_t pos) {
      displayBuffer[pos] = Display7::find_char(digit);
  }
};

