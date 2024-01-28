#include <Arduino.h>
#include "display7.h"
#include "ired.h"
#include "keys.h"

unsigned long currentMillis = 0;

uint8_t channel = 0;

char displayMsg[8];

void keyHandle(Keys::EnumKeyId key, Keys::EnumKeyAction action);

enum ACActionEnum { READ_IR, SEND_IR, NONE };
ACActionEnum acAction = NONE;


void setup()
{
  Serial.begin(115200);

  Keys::setup();
  Display7::setup();
  IRed::setup();
}

void loop()
{
  Keys::loop(keyHandle);
  Display7::loop();

  if (acAction == SEND_IR) {

    sprintf(displayMsg, "0ut %02d", channel);
    Display7::print(displayMsg, 6, 700);

    if (millis() - currentMillis > 2000) {
      IRed::sendIR(channel);
      currentMillis = millis();
    } 

  } else if (acAction == READ_IR) {

    sprintf(displayMsg, "1n%02d", channel);
    Display7::print(displayMsg, 4, 700);
    
    IRed::irReceiverLoop(channel);
  } else if (acAction == NONE) {
    Display7::progressing(750);

  }
}


void keyHandle(Keys::EnumKeyId key, Keys::EnumKeyAction action) {

  if (key == Keys::BUTTON_A) {

    Serial.println("SEND IR");
    Display7::clear();
    acAction = SEND_IR;

  } else if (key == Keys::BUTTON_B) {

    Serial.println("READ IR");
    Display7::clear();
    acAction = READ_IR;

  } else if (key == Keys::BUTTON_C) {
    Serial.println("CHANNEL IR");
    channel++;

    if (channel >= 5)
      channel = 0;

  }

}
