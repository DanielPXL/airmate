#include <stdint.h>
#include <DHT.h>
#include "Pins.h"

#define DHTTYPE DHT11


//Messwerte
int32_t g_temperature = 20;
int32_t g_humidity = 55;
int32_t g_co2ppm = 1200;

//Schwellwerte 
const int32_t HUMIDITY_THRESHOLD = 60;
const int32_t CO2THRESHOLD = 1000;
const int32_t DEWPOINT_MARGIN = 2; 

bool g_buttonPressed = false;

DHT dht(DTH11_PIN, DHTTYPE);

void sensors_setup() {
  // dht sensor setup
  dht.begin();

  // button setup
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // CO2 sensor setup
}

void sensors_update() {
  // Read DHT11 and update g_temperature and g_humidity
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (!isnan(temp) && !isnan(hum)) {
    g_temperature = (int32_t)temp;
    g_humidity = (int32_t)hum;
  }

  //Button prüfen
  if (digitalRead(BUTTON_PIN) == LOW) {
    g_buttonPressed = true;
  } else {
    g_buttonPressed = false;
  }

  /* 
    Read CO2 sensor and update g_co2ppm
    (hier: Dummywert)
  */ 
  g_co2ppm = 1200;  // TODO: Echten Sensor einbinden


  // If shouldOpen(), call window_startOpening();
  if (g_buttonPressed || sensors_shouldOpen()) {
    window_startOpening();
  }
}

bool sensors_shouldOpen() {
  // Taupunkt berechnen etc.
  float temp = (float)g_temperature;
  float hum = (float)g_humidity;

  //vereinfachte Magnus-Formel zum Berechnen des Taupunkts
  float a = 17.62;
  float b = 243.12;
  float gamma = log(hum / 100.0) + (a * temp) / (b + temp);
  float dewPoint = (b * gamma) / (a - gamma);

  // TODO: Auch Wetter (zb g_weatherTemperature) miteinberechnen? 

  // Fenster öffnen bei hoher Luftfeuchtigkeit,
  // Taupunkt sehr nah an Temperatur,
  // oder hoher CO₂-Konzentration
  return (
    hum >= HUMIDITY_THRESHOLD ||
    (temp - dewPoint) < DEWPOINT_MARGIN ||
    g_co2ppm >= CO2THRESHOLD
  );
}
