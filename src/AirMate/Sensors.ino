#include <stdint.h>

int32_t g_temperature = 20;
int32_t g_humidity = 40;
int32_t g_co2ppm = 1000;

void sensors_setup() {
  // DHT11 setup
  // CO2 sensor setup
}

void sensors_update() {
  // Read DHT11 and update g_temperature and g_humidity
  // Read CO2 sensor and update g_co2ppm
  // If button pressed, call window_startOpening();
  // If shouldOpen(), call window_startOpening();
}

bool sensors_shouldOpen() {
  // Taupunkt berechnen etc.
  // Auch Wetter (zb g_weatherTemperature) miteinberechnen?
  return g_humidity > 50;
}
