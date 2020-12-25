#include <ESP8266WiFi.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "WiFiClientHelper.h"
#include "ClockDisplay.h"

// WiFi credentials
const char *ssid                    = "<SSID>";
const char *password                = "<PASSWORD>";

// servers
const char *weatherServer           = "api.data.gov.sg";
const char *sunriseSunsetServer     = "api.sunrise-sunset.org";

// NTP parameters
const long utcOffsetInSeconds       = 8 * 60 * 60; // Singapore UTC +8 offset
const unsigned long updateInterval  = 60 * 60 * 1000; // 1 hr

// create NTPClient instance
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "sg.pool.ntp.org", utcOffsetInSeconds, updateInterval);

// WiFi client to get data from APIs
WiFiClientHelper wifiClient;

// data model of the clock
ClockDisplay display;
unsigned long displayUpdatedTime;

// sunrise & sunset data
unsigned long sunriseTime, sunsetTime;
bool hasSunriseSunsetData = false;

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
  updateDisplay();
}

void loop() {
  // put your main code here, to run repeatedly:
  auto epochTime = timeClient.getEpochTime();
  if (epochTime != displayUpdatedTime && epochTime % 60 == 0) {
    // only update once at the start of each minute
    updateDisplay();
  }

  display.updateAnimations();
}

void updateDisplay() {
  timeClient.update();
  auto hours = timeClient.getHours();
  auto minutes = timeClient.getMinutes();

  if (hours == 0 && minutes == 0) hasSunriseSunsetData = false;
  if (!hasSunriseSunsetData) updateSunriseSunsetTime();

  bool isDay = timeClient.getGmtEpochTime() > sunriseTime && timeClient.getGmtEpochTime() < sunsetTime;
  display.updateDisplay(hours, minutes, isDay, getWeather());
  displayUpdatedTime = timeClient.getEpochTime();
}

time_t tmConvert_t(int YYYY, byte MM, byte DD, byte hh, byte mm, byte ss) {
  tmElements_t tmSet;
  tmSet.Year = YYYY - 1970;
  tmSet.Month = MM;
  tmSet.Day = DD;
  tmSet.Hour = hh;
  tmSet.Minute = mm;
  tmSet.Second = ss;
  return makeTime(tmSet);
}

time_t getEpochFromTimeStamp(const char* timeStamp) {
  int YYYY;
  byte MM, DD, hh, mm, ss;

  sscanf(timeStamp, "%d-%hhd-%hhdT%hhd:%hhd:%hhd", &YYYY, &MM, &DD, &hh, &mm, &ss);

  return tmConvert_t(YYYY, MM, DD, hh, mm, ss);
}

void updateSunriseSunsetTime() {
  if (!wifiClient.tryHttpGet(sunriseSunsetServer, "/json?lat=1.3521&lng=103.8198&date=" + timeClient.getFormattedDate() + "&formatted=0")) {
    return;
  }

  // Allocate the JSON document
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const size_t capacity = 768;
  DynamicJsonDocument doc(capacity);

  if (!wifiClient.tryDeserializeJson(doc)) {
    return;
  }

  sunriseTime = getEpochFromTimeStamp(doc["results"]["nautical_twilight_begin"]);
  sunsetTime = getEpochFromTimeStamp(doc["results"]["nautical_twilight_end"]);
  hasSunriseSunsetData = true;

  //Serial.print("Sunrise time: ");
  //Serial.println(sunriseTime);

  //Serial.print("Sunset time: ");
  //Serial.println(sunsetTime);
  
  //Serial.print("Epoch time: ");
  //Serial.println(timeClient.getGmtEpochTime());
}

String getWeather() {
  if (!wifiClient.tryHttpGet(weatherServer, "/v1/environment/2-hour-weather-forecast")) {
    return String();
  }

  // Allocate the JSON document
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const size_t capacity = 8192;
  DynamicJsonDocument doc(capacity);

  if (!wifiClient.tryDeserializeJson(doc)) {
    return String();
  }

  // Extract values
  return doc["items"][0]["forecasts"][1]["forecast"].as<String>(); // forecast for Bedok
}
