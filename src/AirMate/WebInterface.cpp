#include "website/dist/index_html.h"
#include <WebServer.h>
#include "Log.h"
#include "Window.h"
#include "Weather.h"
#include "Sensors.h"
#include "WebInterface.h"

WebServer server(80);

void handleIndex() {
  server.send_P(
    200, 
    "text/html\r\nContent-Encoding: gzip", /* Extrem ekelhaft */
    reinterpret_cast<const char*>(index_html),
    index_html_len
  );
}

void sendData() {
  char dataJson[512];
  sprintf(
    dataJson,
    "{\"sensors\": {\"temperature\": %f, \"humidity\": %f, \"co2ppm\": %f}, \"weather\": {\"temperature\": %f, \"humidity\": %f, \"dewpoint\": %f, \"apparentTemperature\": %f}, \"state\": \"%s\", \"autoEnabled\": %d}",
    g_temperature, g_humidity, g_co2ppm, g_weatherTemperature, g_weatherHumidity, g_weatherDewPoint, g_weatherApparentTemperature, window_getState(), g_autoEnabled 
  );
  server.send(
    200,
    "application/json",
    dataJson
  );
}

void handleButtonPush() {
  window_buttonToggle();
  sendData();
}

void handleSetAuto() {
  g_autoEnabled = true;
  sendData();
}

void handleResetAuto() {
  g_autoEnabled = false;
  sendData();
}

void webinterface_setup() {
  server.on("/", handleIndex);
  server.on("/data", sendData);
  server.on("/buttonPushed", HTTP_POST, handleButtonPush);
  server.on("/setAuto", HTTP_POST, handleSetAuto);
  server.on("/resetAuto", HTTP_POST, handleResetAuto);

  server.begin();
  LOG("Webserver started");
}

void webinterface_loop() {
  server.handleClient();
}
