
#include "ired.h"


namespace IRed {

  IRsend irsend(kIrLed); 

  void setup() {
    irsend.begin();
    ///Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  }

  void loop() {

  }

  void sendIR(uint16_t *raw, uint16_t size) {

    //Serial.println("a rawData capture from IRrecvDumpV2");
    irsend.sendRaw(raw, size, 38);  // Send a raw data capture at 38kHz.

  }
  
};
