#include "web.h"

namespace Web
{
  String _irdomId;

  void setup(String irdomId)
  {

    _irdomId = irdomId;
    WiFi.begin("HUAWEI-5R42NS_HiLink", "");

    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
    }
  }


  void registerDevice() {
    Serial.println("Registering device");

    if (WiFi.status() == WL_CONNECTED)
    {

      WiFiClient client;
      HTTPClient http;

      http.begin(client, "http://192.168.3.19:8090/register");
      http.addHeader("Content-Type", "text/plain");
      http.addHeader("x-irdom-id", _irdomId);

      int httpCode = http.PATCH("");
      String payload = http.getString();

      Serial.println(httpCode);
      Serial.println(payload);

      http.end();
    }
    else
    {
      Serial.println("Error in WiFi connection");
    }

  }

  void loop()
  {

    Serial.println("Requesting...");

    if (WiFi.status() == WL_CONNECTED)
    {

      WiFiClient client;
      HTTPClient http;

      http.begin(client, "http://192.168.3.19:8090/api/hello");
      http.addHeader("Content-Type", "text/plain");

      int httpCode = http.GET();
      String payload = http.getString();

      Serial.println(httpCode);
      Serial.println(payload);

      http.end();
    }
    else
    {
      Serial.println("Error in WiFi connection");
    }

  }

 

};
