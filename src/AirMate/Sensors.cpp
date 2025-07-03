#include "esp32-hal-gpio.h"
#include <stdint.h>
#include <DHT.h> // DHT sensor library von Adafruit
#include <MHZ.h> // MH-Z CO2 Sensors library von Tobias Schürg etc
// und EspSoftwareSerial library von Dirk Kaar etc
// -------------------------------import für WifWaf Implementierung---
#include <HardwareSerial.h>
#include  "MHZ19.h" //MH-Z 19 CO2 Sensor library von WifWaf
//--------------------------------------------------------------------

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
float g_dewPoint = 20;

//Schwellwerte 
const float HUMIDITY_THRESHOLD = 60;
const float CO2THRESHOLD = 1000;
const float OPTIMAL_TEMP = 22;
const float MAX_WINDSPEED = 35;

DHT dht(DTH11_PIN, DHTTYPE);
MHZ co2(MH_Z19_RX, MH_Z19_TX, MHZ19C);

//----------------------------------------Co2 mit WifWaf Library -----------------------------
HardwareSerial mhzSerial(2); // Erstelle eine Instanz von HardwareSerial für UART2
MHZ19 mhz19; // Erstelle ein Objekt der MHZ19-Klasse
//--------------------------------------------------------------------------------------------


void sensors_setup() {
  // button setup
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);

  // Reed sensor setup
  pinMode(REEDSENSOR_PIN, INPUT);

  // dht sensor setup
  dht.begin();

  //--------------------------------------------Setup CO2-----------------------------
  mhzSerial.begin(9600, SERIAL_8N1, MH_Z19_RX, MH_Z19_TX); // Serielle Kommunikation mit dem MH-Z19 starten
  mhz19.begin(mhzSerial); // MH-Z19-Bibliothek mit der seriellen Schnittstelle verbinden

  // Optional: Sensor kalibrieren (nur einmalig, wenn nötig!)
  mhz19.calibrate(); // Automatische Basiskalibrierung (ABC) einschalten
  // mhz19.calibrateZero(); // Kalibrierung auf 400ppm (nur wenn der Sensor in frischer Luft ist!)

  //---------------------------------------------------------------------------------

}

// Wird nur alle 10s ausgeführt
void sensors_update() {
  // Read DHT11 and update g_temperature and g_humidity
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (!isnan(temp) && !isnan(hum)) {
    g_temperature = temp;
    g_humidity = hum;
    g_dewPoint = sensors_taupunkt(g_temperature, g_humidity);
  }

  if (g_state == State::Closed && digitalRead(REEDSENSOR_PIN) == LOW) {
    //Das Fenster sitzt nicht am Ramen sollte aber zu sein
    g_state == State::Alarm;
  }

  g_co2ppm = mhz19.getCO2();

  // If shouldOpen(), do it
  if (sensors_shouldOpen()) {
    window_startOpening();
  }
}

bool sensors_shouldOpen() {
  if (!g_autoEnabled) {
    return false;
  }

  float deltaTP = g_dewPoint - g_weatherDewPoint;

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

  // C02 auf unter 950ppm halten
  if (g_co2ppm >= 1200) {
    return true;
  }

  // Taupunkt
  if (deltaTP > SCHALTminDewPoint) {
    return true;
  }

  // Temperatur auf 22°C regeln
  if (g_temperature >= OPTIMAL_TEMP && g_weatherTemperature <= OPTIMAL_TEMP) {
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

// Wird so oft wie möglich ausgeführt
void sensors_loop() {
  // Button prüfen
  // Solange Button gedrückt wird, wird kein weiterer Toggle ausgelöst
  static bool buttonOldPush = false;
  bool buttonPush = digitalRead(BUTTON_PIN) == HIGH;
  if (buttonPush && !buttonOldPush) {
    // Button wurde gedrückt
    window_buttonToggle();
  }

  // Buttonstatus speichern
  buttonOldPush = buttonPush;
}
