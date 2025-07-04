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
    index_html_len);
}

void sendData() {
  char dataJson[1024];
  sprintf(
    dataJson,
    R"json(
      {
        "sensors": {
          "temperature": %f,
          "humidity": %f,
          "co2ppm": %i,
          "dewpoint": %f
        },
        "weather": {
          "temperature": %f,
          "humidity": %f,
          "dewpoint": %f,
          "apparentTemperature": %f,
          "windSpeed": %f,
          "precipitation": %f,
          "isDay": %d
        },
        "state": "%s",
        "autoEnabled": %d,
        "closeTime": %u
      }
    )json",
    g_temperature, g_humidity, g_co2ppm, g_dewPoint,
    g_weatherTemperature, g_weatherHumidity, g_weatherDewPoint,
    g_weatherApparentTemperature, g_weatherWindSpeed, g_weatherPrecipitation,
    g_weatherIsDay, window_getState(), g_autoEnabled, g_closeTimeGoal / (60 * 1000)
  );

  server.send(
    200,
    "application/json",
    dataJson);
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

void handleSetCloseTime(uint32_t minTime) {
  g_closeTimeGoal = minTime * 60 * 1000;
  sendData();
}

void webinterface_setup() {
  server.on("/", handleIndex);
  server.on("/data", sendData);
  server.on("/buttonPushed", HTTP_POST, handleButtonPush);
  server.on("/setAuto", HTTP_POST, handleSetAuto);
  server.on("/resetAuto", HTTP_POST, handleResetAuto);

  // Ich hab (auf die Schnelle) keinen besseren Weg gefunden,
  // Daten an den Server zu senden, also machen wir das so
  server.on("setCloseTime3", HTTP_POST, []() { handleSetCloseTime(3); });
  server.on("setCloseTime5", HTTP_POST, []() { handleSetCloseTime(5); });
  server.on("setCloseTime10", HTTP_POST, []() { handleSetCloseTime(10); });
  server.on("setCloseTime15", HTTP_POST, []() { handleSetCloseTime(15); });
  server.on("setCloseTime20", HTTP_POST, []() { handleSetCloseTime(20); });

  server.begin();
  LOG("Webserver started");
}

void webinterface_loop() {
  server.handleClient();
}
