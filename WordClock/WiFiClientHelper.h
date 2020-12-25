#ifndef _WIFICLIENTHELPER_H_
#define _WIFICLIENTHELPER_H_

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

class WiFiClientHelper
{
private:
  WiFiClientSecure wifiClient;

public:
  WiFiClientHelper() {
    wifiClient.setInsecure();
  };

  bool tryHttpGet(String server, String request);
  bool tryDeserializeJson(DynamicJsonDocument& doc);
};
#endif