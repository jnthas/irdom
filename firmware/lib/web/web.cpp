#include "web.h"

namespace Web
{

  String json;

  String _irdomId;  //change to static fixed char[]


  void setup(String irdomId)
  {

    _irdomId = irdomId;
    WiFi.begin("HUAWEI-5R42NS_HiLink", "07627983");

    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
    }
  }


  bool registerDevice() {
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

  void uploadCode(String json) {
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
