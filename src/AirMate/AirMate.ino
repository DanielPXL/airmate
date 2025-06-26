#include "Window.h"
#include "Weather.h"
#include "WIFILogin.h"
#include "WebInterface.h"
#include "Sensors.h"

void setup() {
  Serial.begin(9600);
  wifi_connect();
  webinterface_setup();
  window_setup();
  sensors_setup();

  // Bei startup einmal schon updaten, sonst wird erst 30s gewartet
  weather_update();
  sensors_update();
}

void loop() {
  static uint64_t lastWeatherUpdate = 0;
  static uint64_t lastSensorUpdate = 0;
  uint64_t now = millis();
  if (now - lastWeatherUpdate > 60000) {
    lastWeatherUpdate = now;
    weather_update();
  }

  if (now - lastSensorUpdate > 10000) {
    lastSensorUpdate = now;
    sensors_update();
  }

  window_loop();
  webinterface_loop();
}