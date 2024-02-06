#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>


namespace Web {

  void setup(String irdomId);
  void registerDevice();
  void uploadCode(String json);
  void loop();

};