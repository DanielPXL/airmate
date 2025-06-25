#include <stdint.h>
#include <DHT.h> // DHT sensor library von Adafruit
#include "Pins.h"

#define DHTTYPE DHT11

#define SCHALTmin 5.0 // minimaler Taupunktunterschied, bei dem das Fenster öffnet

bool g_autoEnabled = true;

//Messwerte
float g_temperature = 20;
float g_humidity = 55;
float g_co2ppm = 1200;

//Schwellwerte 
const float HUMIDITY_THRESHOLD = 60;
const float CO2THRESHOLD = 1000;

DHT dht(DTH11_PIN, DHTTYPE);

void sensors_setup() {
  // button setup
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // dht sensor setup
  dht.begin();

  // CO2 sensor setup
}

// Button
bool button_push = false;
bool button_oldstat = false;

void sensors_update() {
  // Read DHT11 and update g_temperature and g_humidity
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (!isnan(temp) && !isnan(hum)) {
    g_temperature = temp;
    g_humidity = hum;
  }

  // Button prüfen
  // Solange Button gedrückt wird, wird kein weiterer Toggle ausgelöst
  button_push = digitalRead(BUTTON_PIN) == LOW; // LOW = gedrückt (INPUT_PULLUP)

  if (button_push && !button_oldstat) {
    // Button wurde gedrückt
    window_buttonToggle();
  }



// Taupunktberechnung
  float Taupunkt_1;
  float Taupunkt_2;
  float DeltaTP;

  Taupunkt_1 = taupunkt(g_temperature, g_humidity);
  Taupunkt_2 = g_weatherDewPoint;
  DeltaTP = Taupunkt_1 - Taupunkt_2;

  // Buttonstatus speichern
  button_oldstat = button_push;


  /* 
    Read CO2 sensor and update g_co2ppm
    (hier: Dummywert)
  */ 
  g_co2ppm = 1200;  // TODO: Echten Sensor einbinden

  // If shouldOpen(), do it
  if (sensors_shouldOpen()) {
    // TODO
  }
}

bool sensors_shouldOpen() {
  if (!g_autoEnabled) {
    return false;
  }




  // TODO
}


float taupunkt(float t, float r) {
  float a, b;

  if (t >= 0) {
    a = 7.5;
    b = 237.3;
  } else if (t < 0) {
    a = 7.6;
    b = 240.7;
  }

  // Sättigungsdampfdruck in hPa
  float sdd = 6.1078 * pow(10, (a*t)/(b+t));

  // Dampfdruck in hPa
  float dd = sdd * (r/100);

  // v-Parameter
  float v = log10(dd/6.1078);

  // Taupunkttemperatur (°C)
  float tt = (b*v) / (a-v);
  return tt;
}