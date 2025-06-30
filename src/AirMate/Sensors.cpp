#include "esp32-hal-gpio.h"
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

DHT dht(DTH11_PIN, DHTTYPE);
MHZ co2(MH_Z19_RX, MH_Z19_TX, MHZ19C);

void sensors_setup() {
  // button setup
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);

  // Reed sensor setup
  pinMode(REEDSENSOR_PIN, INPUT);

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

  if (g_state == State::Closed && digitalRead(REEDSENSOR_PIN) == LOW) {
    //Das Fenster sitzt nicht am Ramen sollte aber zu sein
    g_state == State::Alarm;
  }

  // CO2 Sensor lesen
  // Funktioniert nur, wenn Logging ausgeschaltet ist
  // (siehe Log.h für mehr Informationen)
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
  float Taupunkt_1;
  float Taupunkt_2;
  float DeltaTP;

  Taupunkt_1 = sensors_taupunkt(g_temperature, g_humidity);
  Taupunkt_2 = g_weatherDewPoint;
  DeltaTP = Taupunkt_1 - Taupunkt_2;

  // TODO
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
