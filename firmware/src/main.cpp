#include <Arduino.h>
#include <Esp.h>
#include <MD5Builder.h>
#include <ArduinoJson.h>
#include "display7.h"
#include "ired.h"
#include "keys.h"
#include "web.h"
#include "errorhandler.h"

unsigned long currentMillis = 0;

uint8_t channel = 0;

char displayMsg[8];

void keyHandle(Keys::EnumKeyId key, Keys::EnumKeyAction action);
void checkSchedule();
String generateIrdomId();





enum ACActionEnum
{
  READ_IR,
  SEND_IR,
  NONE
};
ACActionEnum acAction = NONE;

MD5Builder md5;

JsonDocument doc;
String json;

void setup()
{
  Serial.begin(115200);
  String irdomId = generateIrdomId();

  Keys::setup();
  Display7::setup();
  IRed::setup();

  Web::setup(irdomId);

  Serial.println("irdomId: " + irdomId);

  bool success = Web::registerDevice();

  if (!success) {
    ErrorHandler::handleError(ErrorHandler::BACKEND_REGISTER_FAILED, "Failed registering device!!!");
  }

  // Serial.println("Info");
  // Serial.println(ESP.getChipId());
  // Serial.println(ESP.getResetInfo());
  // Serial.println(ESP.getSketchMD5());
  // Serial.println(ESP.getSdkVersion());
  // Serial.println(ESP.getSketchSize());
  // Serial.println(WiFi.macAddress());
}

void loop()
{
  Keys::loop(keyHandle);
  Display7::loop();

  checkSchedule();

  if (acAction == SEND_IR)
  {

    sprintf(displayMsg, "0ut %02d", channel);
    Display7::print(displayMsg, 6, 700);

    if (millis() - currentMillis > 2000)
    {
      IRed::sendIR(channel);
      currentMillis = millis();
    }
  }
  else if (acAction == READ_IR)
  {

    sprintf(displayMsg, "1n%02d", channel);
    Display7::print(displayMsg, 4, 700);

    if (IRed::irReceiverLoop(channel))
    {
      IRed::DeviceButton d = IRed::getDecodedButton();

      Serial.println("Decoded: ");
      Serial.println(d.size);
      Serial.println(d.protocol);

      doc[channel]["slot"] = channel;
      doc[channel]["protocol"] = d.protocol;
      doc[channel]["size"] = d.size;
      doc[channel]["buttonCode"] = d.hexButtonCode;

      serializeJson(doc, json);
      serializeJson(doc, Serial);

      Web::uploadCode(json.c_str());
    }
  }
  else if (acAction == NONE)
  {
    Display7::progressing(750);
  }
}

void checkSchedule() {

  if (millis() - currentMillis >= SCHEDULE_QUERY_INTERVAL)
  {
    int httpCode = Web::querySchedule(json);

    if (httpCode == 200) {

      IRed::DeviceButton deviceButton;

      deserializeJson(doc, json);

      deviceButton.protocol = static_cast<decode_type_t>(doc["protocol"].as<uint16_t>());
      deviceButton.size = doc["size"].as<uint16_t>();
      deviceButton.hexButtonCode = doc["signal"].as<String>();

      for (uint16_t i = 2; i<deviceButton.hexButtonCode.length(); i+=2) {

        String hex = deviceButton.hexButtonCode.substring(i, i+2);  
        uint8_t number = strtol(hex.c_str(), NULL, 16);
        deviceButton.buttonCode[(i-2)/2] = number;

        //Serial.print("Digit: ");
        //Serial.println(deviceButton.buttonCode[(i-2)/2], HEX);
      }

      IRed::sendIR(deviceButton);
    }

    
    currentMillis = millis();
  }
}

void keyHandle(Keys::EnumKeyId key, Keys::EnumKeyAction action)
{

  if (key == Keys::BUTTON_A)
  {

    Serial.println("SEND IR");
    Display7::clear();
    acAction = SEND_IR;
  }
  else if (key == Keys::BUTTON_B)
  {

    Serial.println("READ IR");
    Display7::clear();
    acAction = READ_IR;
  }
  else if (key == Keys::BUTTON_C)
  {
    Serial.println("CHANNEL IR");
    channel++;

    if (channel >= 5)
      channel = 0;
  }
}

String generateIrdomId()
{
  md5.begin();
  String chipId = String(ESP.getChipId());
  md5.add(chipId + "+" + WiFi.macAddress());
  md5.calculate();

  return md5.toString();
}
