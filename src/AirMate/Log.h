#ifndef LOG_H
#define LOG_H
#include <HardwareSerial.h>

// Logging muss ausgeschaltet werden, wenn der CO2-Sensor
// benutzt wird, weil wir (ohne davon zu wissen) den CO2-Sensor
// an die selben UART-Pins angelötet haben, die auch
// vom USB Controller benutzt werden

#define LOGGING_ENABLED 0

#if LOGGING_ENABLED
#define LOG(...) Serial.printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

#endif