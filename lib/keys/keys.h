#pragma once

#include <Arduino.h>



#define KEYS_PIN (17)  // A0 of ESP8266


//read from board
//btn0 = 1000
//btn1 = 745
//btn2 = 545
#define BUTTON_A_THRESHOLD (900)
#define BUTTON_B_THRESHOLD (700)
#define BUTTON_C_THRESHOLD (500)



namespace Keys {

  enum EnumKeyId { BUTTON_A, BUTTON_B, BUTTON_C };
  enum EnumKeyAction { PRESSED, RELEASED };

  typedef void (*KeyEvent)(Keys::EnumKeyId, Keys::EnumKeyAction);

  void setup();
  void loop(KeyEvent keyEvent);

};