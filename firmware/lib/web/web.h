#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

#include "ired.h"


namespace Web {

  void setup(String irdomId);
  bool registerDevice();
  void uploadCode(String json);
  void loop();

};