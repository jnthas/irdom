#include <Arduino.h>
#include <Esp.h>
#include <MD5Builder.h>
#include "display7.h"
#include "ired.h"
#include "keys.h"
#include "web.h"

unsigned long currentMillis = 0;

uint8_t channel = 0;

char displayMsg[8];

void keyHandle(Keys::EnumKeyId key, Keys::EnumKeyAction action);
String generateIrdomId();

enum ACActionEnum { READ_IR, SEND_IR, NONE };
ACActionEnum acAction = NONE;

MD5Builder md5;

void setup()
{
  Serial.begin(115200);
  String irdomId = generateIrdomId();


  Keys::setup();
  Display7::setup();
  IRed::setup();

  Web::setup(irdomId);


  Serial.println("Info");
  Serial.println(ESP.getChipId());
  Serial.println(ESP.getResetInfo());
  Serial.println(ESP.getSketchMD5());
  Serial.println(ESP.getSdkVersion());
  Serial.println(ESP.getSketchSize());
  Serial.println(WiFi.macAddress());
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

    Web::registerDevice();

  }
}

String generateIrdomId() {
  md5.begin();
  Serial.println("irdomId:");
  Serial.println(String(ESP.getChipId()).concat("+") + WiFi.macAddress());
  md5.add(ESP.getChipId() + "+" + WiFi.macAddress());
  md5.calculate();

  return md5.toString();
}

