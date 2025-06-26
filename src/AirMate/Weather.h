#ifndef WEATHER_H
#define WEATHER_H

extern float g_weatherTemperature;
extern float g_weatherHumidity;
extern float g_weatherDewPoint;
extern float g_weatherApparentTemperature;

void weather_update();

#endif