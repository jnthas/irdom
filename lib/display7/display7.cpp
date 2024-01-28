#include "display7.h"

namespace Display7 {
  
  bool currentDigit1 = true;

  unsigned long currentMillisFrameRate = 0;
  unsigned long currentMillisDelay = 0;
 
  char *displayBuffer;
  uint8_t currentSize;
  uint16_t currentDelay;
  uint8_t currentIndex = 0;

  void setup ()
  {
    // Setup pins as Outputs
    pinMode(DISP_LATCH_PIN, OUTPUT);
    pinMode(DISP_CLOCK_PIN, OUTPUT);
    pinMode(DISP_DATA_PIN, OUTPUT);

    pinMode(DISP_DIG1_PIN, OUTPUT);
    pinMode(DISP_DIG2_PIN, OUTPUT);
  }

  void loop()
  {
    bufferControl();
  }


  void print(const char* content) {
    print(content, 2, 1000);

  }


  void print(const char* content, uint8_t size, uint16_t delay) {

    if (content != displayBuffer) {
      currentMillisDelay = 0;
      currentSize = size;
      currentDelay = delay;
      currentIndex = 0;

      displayBuffer = (char*)content;

      if (currentSize%2 == 1) {
        displayBuffer[size] = ' ';
        currentSize++;
      }
    }

  } 

  void bufferControl() {

    if (displayBuffer) {
      if ((millis() - currentMillisDelay) >= currentDelay) {
        
        currentIndex += 2;

        if (currentIndex >= currentSize) {
          currentIndex = 0;
        }

        currentMillisDelay = millis();
      }

      renderContent(getCharCode(displayBuffer[currentIndex]), getCharCode(displayBuffer[currentIndex+1]));
    }
  }


  void progressing(uint16_t  delay) {
    print(".  .", 4, delay);
  }


  void clear() {
    print(" ", 1, 1000);
  }

  void renderContent(uint8_t char1, uint8_t char2) {    

    if ((millis() - currentMillisFrameRate) >= DISPLAY_FRAMERATE) {
      
      digitalWrite(DISP_LATCH_PIN, LOW);

      if (currentDigit1) {
        digitalWrite(DISP_DIG1_PIN, LOW);
        digitalWrite(DISP_DIG2_PIN, HIGH);
        shiftOut(DISP_DATA_PIN, DISP_CLOCK_PIN, LSBFIRST, char1);
      } else {
        digitalWrite(DISP_DIG1_PIN, HIGH);
        digitalWrite(DISP_DIG2_PIN, LOW);
        shiftOut(DISP_DATA_PIN, DISP_CLOCK_PIN, LSBFIRST, char2);
      }

      digitalWrite(DISP_LATCH_PIN, HIGH);

      currentDigit1 = !currentDigit1;
      currentMillisFrameRate = millis();

    }
  }



  uint8_t getCharCode(char digit) {
      for (uint8_t b = 0; b<CHAR_ARR_SIZE; b++) 
      {
          if (DisplayChars[b][1] == digit)
              return Display7::DisplayChars[b][0];
      }

      return Display7::DisplayChars[0][0];
  }

};

