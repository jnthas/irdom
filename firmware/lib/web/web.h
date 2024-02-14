#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include "ired.h"



#define SCHEDULE_QUERY_INTERVAL (1*60*1000)

namespace Web {

  void setup(String irdomId);
  bool registerDevice();
  bool uploadCode(const char *json);
  void loop();

  int querySchedule(String &payload);

};