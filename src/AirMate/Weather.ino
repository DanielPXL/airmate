#include <stdint.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h> // Benötigt ArduinoJson library von Tobias Schürg etc.

const char WEATHER_URL[] = "https://api.open-meteo.com/v1/forecast?latitude=53.147755555522835&longitude=8.183846055172747&current=temperature_2m,relative_humidity_2m,dew_point_2m,apparent_temperature";

float g_weatherTemperature = 20;
float g_weatherHumidity = 40;
float g_weatherDewPoint = 15;
float g_weatherApparentTemperature = 18;

void weather_setup() {
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
    Serial.print("Could not GET ");
    Serial.println(WEATHER_URL);
  }

  String jsonString = httpClient.getString();
  weather_parseJson(jsonString);
}

void weather_parseJson(String jsonString) {
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, jsonString.c_str());

  if (error) {
    Serial.print("JSON parse failed: ");
    Serial.println(error.f_str());
    return;
  }

  JsonObject current = doc["current"];
  g_weatherTemperature = current["temperature_2m"];
  g_weatherHumidity = current["relative_humidity_2m"];
  g_weatherDewPoint = current["dew_point_2m"];
  g_weatherApparentTemperature = current["apparent_temperature"];
}
