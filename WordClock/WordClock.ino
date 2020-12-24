#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// WiFi credentials
const char *ssid      = "<SSID>";
const char *password  = "<PASSWORD>";

// NTP parameters
const long utcOffsetInSeconds       = 8 * 60 * 60; // Singapore UTC +8 offset
const unsigned long updateInterval  = 60 * 60 * 1000; // 1 hr

// Create NTPClient instance
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "sg.pool.ntp.org", utcOffsetInSeconds, updateInterval);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  Serial.println ( "Connected" );

  timeClient.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  timeClient.update();
  timeClient.getFormattedTime();
}
