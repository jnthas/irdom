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

  int querySchedule(String &payload)
  {
    Serial.println("Querying schedule");
    int httpCode = 0;

    if (WiFi.status() == WL_CONNECTED)
    {

      WiFiClient client;
      HTTPClient http;

      http.begin(client, "http://192.168.3.19:8090/schedule");
      http.addHeader("Content-Type", "text/plain");
      http.addHeader("x-irdom-id", _irdomId);

      httpCode = http.GET();
      Serial.printf("Response code: %d\n", httpCode);
      payload = http.getString();
      Serial.println(payload);

      http.end();
    }

    return httpCode;
  }

  bool registerDevice()
  {
    Serial.println("Registering device");

    if (WiFi.status() == WL_CONNECTED)
    {

      WiFiClient client;
      HTTPClient http;

      http.begin(client, "http://192.168.3.19:8090/register");
      http.addHeader("Content-Type", "text/plain");
      http.addHeader("x-irdom-id", _irdomId);

      int httpCode = http.PATCH("");
      Serial.printf("Response code: %d\n", httpCode);
      String payload = http.getString();
      Serial.println(payload);

      http.end();
      return (httpCode == 200);
    }

    return false;
  }

  void loop()
  {
  }

  void uploadCode(String json)
  {
    Serial.println("Updating button code");
    Serial.println(json);

    if (WiFi.status() == WL_CONNECTED)
    {

      WiFiClient client;
      HTTPClient http;

      http.begin(client, "http://192.168.3.19:8090/button-code");
      http.addHeader("Content-Type", "application/json");
      http.addHeader("x-irdom-id", _irdomId);

      int httpCode = http.POST(json);
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
