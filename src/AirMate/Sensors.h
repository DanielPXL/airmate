#ifndef SENSORS_H
#define SENSORS_H

extern bool g_autoEnabled;

extern float g_temperature;
extern float g_humidity;
extern float g_co2ppm;

void sensors_setup();
void sensors_update();

bool sensors_shouldOpen();
float sensors_taupunkt(float t, float r);

#endif