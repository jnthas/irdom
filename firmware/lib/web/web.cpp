#include "web.h"

namespace Web
{
  String _irdomId; // change to static fixed char[]
  unsigned long currentMillis = 0;

  void setup(String irdomId)
  {
    _irdomId = irdomId;
    WiFi.mode(WIFI_STA);
    WiFi.begin("HUAWEI-5R42NS_HiLink", "");

    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print("*");
    }

    Serial.print("\nIP Address: ");
    Serial.println(WiFi.localIP());
  }


  int makeRequest(const char *type, const char *path, const char *payload, String &response) {

    int httpCode = 0;

    if (WiFi.status() == WL_CONNECTED)
    {

      WiFiClient client;
      HTTPClient http;

      String url = "http://192.168.3.19:8090/" + String(path);

      http.begin(client, url);
      http.addHeader("Content-Type", "application/json");
      http.addHeader("x-irdom-id", _irdomId);

      httpCode = http.sendRequest(type, payload);
      Serial.printf("Response code: %d\n", httpCode);

      if (httpCode > 0) {
        response = http.getString();
        Serial.println(response);
      }

      http.end();
    }

    return httpCode;    
  }


  int querySchedule(String &payload)
  {
    Serial.println("Querying schedule");

    int httpCode = makeRequest("GET", "schedule", "", payload);

    return httpCode;
  }

  bool registerDevice()
  {
    Serial.println("Registering device");

    String response;
    int httpCode = makeRequest("PATCH", "register", "", response);

    return (httpCode == 200);
  }

  void loop()
  {
  }

  bool uploadCode(const char *json)
  {
    Serial.println("Updating button code");

    String response;
    int httpCode = makeRequest("POST", "button-code", json, response);

    return (httpCode == 200);
  }

};
