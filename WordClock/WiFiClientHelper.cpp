#include "WiFiClientHelper.h"

bool WiFiClientHelper::tryHttpGet(String server, String request) {
  // Send HTTP request
  if (wifiClient.connect(server, 443)) {
    //Serial.println("connected to server"); 
    // Make a HTTP request: 
    wifiClient.println("GET " + request + " HTTP/1.0");
    wifiClient.println("Host: " + server); 
    wifiClient.println("Connection: close"); 
    if (wifiClient.println() == 0) {
      Serial.println(F("Failed to send request"));
      return false;
    }
  }
  else {
    Serial.println("unable to connect");
    return false;
  }

  // Check HTTP status
  char status[32] = {0};
  wifiClient.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    return false;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!wifiClient.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    return false;
  }

  return true;
}

bool WiFiClientHelper::tryDeserializeJson(DynamicJsonDocument& doc) {
  // Parse JSON object
  DeserializationError error = deserializeJson(doc, wifiClient);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return false;
  }

  return true;
}