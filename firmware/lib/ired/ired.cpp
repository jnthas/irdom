
#include "ired.h"


namespace IRed {
  
  IRsend irsend(IR_LED_PIN); 
  IRrecv irrecv(IR_RECV_PIN, IR_BUFFER_SIZE, IR_TIMEOUT, true);
  decode_results results;

  struct DeviceButton decodedButton;


  void setup() {
    irsend.begin();
    irrecv.setTolerance(25);
    irrecv.enableIRIn();
  }



  void sendIR(uint8_t index) {

    Serial.print("[SEND] AirCond Type: ");
    Serial.println(decodedButton.protocol);
    //Serial.print("[SEND] Button Code: ");
    //Serial.println(decodedButton.buttonCode, HEX);
    Serial.print("[SEND] Size: ");
    Serial.println(decodedButton.size, DEC);



    bool success = irsend.send(decodedButton.protocol, decodedButton.buttonCode, decodedButton.size);


    if (!success) {
      Serial.println("Error sending IR");

      Serial.print("[SEND] AirCond Type: ");
      Serial.println(decodedButton.protocol);
      //Serial.print("[SEND] Button Code: ");
      //Serial.println(decodedButton.buttonCode, HEX);
      Serial.print("[SEND] Size: ");
      Serial.println(decodedButton.size, DEC);
    }
  }




  void sendIR(uint16_t *raw, uint16_t size) {

    //Serial.println("a rawData capture from IRrecvDumpV2");
    //irsend.sendRaw(raw, size, 38);  // Send a raw data capture at 38kHz.
    bool success = false;



    if (hasACState(results.decode_type)) {  // Does the message require a state[]?
      // It does, so send with bytes instead.
      Serial.println("Has AC state");
      success = irsend.send(results.decode_type, results.state, results.bits / 8);
    } else {  // Anything else must be a simple message protocol. ie. <= 64 bits
      Serial.println("Simple msg protocol");
      success = irsend.send(results.decode_type, results.value, results.bits);
    }


    if (!success) {
      Serial.println("Error sending IR");

      Serial.print("[SEND] AirCond Type: ");
      Serial.println(results.decode_type);
      Serial.print("[SEND] Button Code: ");
      Serial.println(results.value, HEX);
      Serial.print("[SEND] Size: ");
      Serial.println(results.bits, DEC);
    }
  }

  bool irReceiverLoop(uint8_t index) {

    if (irrecv.decode(&results)) {
      
      // Check if we got an IR message that was to big for our capture buffer.
      if (results.overflow)
        errorHandler(EnumDefaultErrors::BufferOverflow);

      // Display the basic output of what we found.
      //Serial.print(resultToHumanReadableBasic(&results));

      
      Serial.print("AirCond Type: ");
      Serial.println(results.decode_type);
      Serial.print("Button Code: ");
      Serial.println(results.value, HEX);
      Serial.print("Size: ");
      Serial.println(results.bits, DEC);


      decodedButton.protocol = results.decode_type;
      decodedButton.size = results.bits;

      if (hasACState(results.decode_type)) {
        decodedButton.hexButtonCode = resultToHexidecimal(&results);
        decodedButton.size = (results.bits / 8); //1 byte size
      }

      return true;
    }

    return false;
  }


  void errorHandler(IRed::EnumDefaultErrors error) {
    Serial.println("Error ");
    Serial.println(error);
  }


  DeviceButton getDecodedButton() {
    return decodedButton;
  }
  
};
