#include <stdint.h>
#include <DHT.h> // DHT sensor library von Adafruit
#include "Pins.h"

#define DHTTYPE DHT11


//Messwerte
float g_temperature = 20;
float g_humidity = 55;
float g_co2ppm = 1200;

//Schwellwerte 
const float HUMIDITY_THRESHOLD = 60;
const float CO2THRESHOLD = 1000;
const float DEWPOINT_MARGIN = 2; 


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
    g_temperature = temp;
    g_humidity = hum;
  }

  //Button prüfen
  bool g_buttonPressed = digitalRead(BUTTON_PIN) == LOW;
  

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
  float temp = g_temperature;
  float hum = g_humidity;

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
