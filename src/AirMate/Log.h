#ifndef LOG_H
#define LOG_H
#include <HardwareSerial.h>

// Logging ausschalten war früher mal wichtig,
// da wir (ausversehen) den CO2-Sensor an UART0
// angeschlossen haben, also die Pins, die auch
// an den USB-Controller angeschlossen sind

#define LOGGING_ENABLED 1

#if LOGGING_ENABLED
#define LOG(...) Serial.printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

#endif