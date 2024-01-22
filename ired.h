#pragma once

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

namespace IRed {


  const uint8_t kIrLed = 4;

  void setup();
  void loop();
  void sendIR(uint16_t *raw, uint16_t size);

};