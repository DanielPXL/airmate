#include <stdint.h>
#include <DHT.h> // DHT sensor library von Adafruit
#include <MHZ.h> // MH-Z CO2 Sensors library von Tobias Schürg etc
// und EspSoftwareSerial library von Dirk Kaar etc
#include "Pins.h"
#include "Weather.h"
#include "Sensors.h"
#include "Window.h"

#define DHTTYPE DHT11

#define SCHALTminDewPoint 5.0 // minimaler Taupunktunterschied, bei dem das Fenster öffnen soll

bool g_autoEnabled = true;

//Messwerte
float g_temperature = 20;
float g_humidity = 55;
int32_t g_co2ppm = 1200;

bool g_buttonOldPush = false;

//Schwellwerte 
const float HUMIDITY_THRESHOLD = 60;
const float CO2THRESHOLD = 1000;
const float OPTIMAL_TEMP = 22;
const float MAX_WINDSPEED = 35;

DHT dht(DTH11_PIN, DHTTYPE);
MHZ co2(MH_Z19_RX, MH_Z19_TX, MHZ19C);

void sensors_setup() {
  // button setup
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);

  // dht sensor setup
  dht.begin();
}

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
  bool buttonPush = digitalRead(BUTTON_PIN) == HIGH;
  if (buttonPush && !g_buttonOldPush) {
    // Button wurde gedrückt
    window_buttonToggle();
  }

  // Buttonstatus speichern
  g_buttonOldPush = buttonPush;

  // CO2 Sensor lesen
#if LOGGING_ENABLED
  g_co2ppm = 404;
#else
  g_co2ppm = co2.readCO2UART();
#endif
  
  // If shouldOpen(), do it
  if (sensors_shouldOpen()) {
    window_startOpening();
  }
}

bool sensors_shouldOpen() {
  if (!g_autoEnabled) {
    return false;
  }

  // Taupunktberechnung
  float taupunkt_1 = sensors_taupunkt(g_temperature, g_humidity);
  float taupunkt_2 = g_weatherDewPoint;
  float deltaTP = taupunkt_1 - taupunkt_2;

  // Öffnungsparameter
  // Nachts nicht lüften
  if (!g_weatherIsDay) {
    return false;
  }

  // Wenns zu windig ist nicht lüften
  if (g_weatherWindSpeed > MAX_WINDSPEED) {
    return false;
  }

  // Wenn Niederschlag nicht lüften
  if (g_weatherPrecipitation > 0) {
    return false;
  }

  // Temperatur auf 22°C regeln
  if (g_temperature >= OPTIMAL_TEMP && g_weatherTemperature <= OPTIMAL_TEMP) {
    return true;
  }

  // Taupunkt
  if (deltaTP > SCHALTminDewPoint || -deltaTP > SCHALTminDewPoint) {
    return true;
  }

  return false;
}

float sensors_taupunkt(float t, float r) {
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
