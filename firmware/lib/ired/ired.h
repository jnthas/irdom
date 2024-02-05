#pragma once

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>


#define IR_LED_PIN (4)
#define IR_RECV_PIN (5)
#define IR_BUFFER_SIZE (256)
#define IR_TIMEOUT (50)

namespace IRed {

  enum EnumDefaultErrors { BufferOverflow };

  struct DeviceButton {
    decode_type_t protocol;
    uint8_t buttonCode[IR_BUFFER_SIZE];
    uint8_t size;
  };



  void setup();
  bool irReceiverLoop(uint8_t index);
  void sendIR(uint16_t *raw, uint16_t size);
  void sendIR(uint8_t index);
  void errorHandler(IRed::EnumDefaultErrors error);

};