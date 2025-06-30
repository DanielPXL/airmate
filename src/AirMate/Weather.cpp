#include <stdint.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h> // Benötigt ArduinoJson library von Tobias Schürg etc.
#include "Log.h"

const char WEATHER_URL[] = "https://api.open-meteo.com/v1/forecast?latitude=53.147755555522835&longitude=8.183846055172747&current=temperature_2m,relative_humidity_2m,dew_point_2m,apparent_temperature,wind_speed_10m,precipitation,is_day";

float g_weatherTemperature = 20;
float g_weatherHumidity = 40;
float g_weatherDewPoint = 15;
float g_weatherApparentTemperature = 18;
float g_weatherWindSpeed = 10;
float g_weatherPrecipitation = 0;
bool g_weatherIsDay = 1;

void weather_parseJson(String jsonString) {
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, jsonString.c_str());

  if (error) {
    LOG("JSON parse failed: %s\n", error.f_str());
    return;
  }

  JsonObject current = doc["current"];
  g_weatherTemperature = current["temperature_2m"];
  g_weatherHumidity = current["relative_humidity_2m"];
  g_weatherDewPoint = current["dew_point_2m"];
  g_weatherApparentTemperature = current["apparent_temperature"];
  g_weatherWindSpeed = current["wind_speed_10m"];
  g_weatherPrecipitation = current["precipitation"];
  g_weatherIsDay = current["is_day"];
}

void weather_update() {
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }

  // Daten updaten
  HTTPClient httpClient;
  httpClient.begin(WEATHER_URL);
  int responseCode = httpClient.GET();
  if (responseCode != 200) {
    LOG("Could not GET %s\n", WEATHER_URL);
  }

  String jsonString = httpClient.getString();
  weather_parseJson(jsonString);
}
