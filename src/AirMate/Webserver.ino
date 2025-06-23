#include <WebServer.h>
#include "website/dist/index_html.h"

WebServer server(80);

void handleIndex() {
  server.send_P(
    200, 
    "text/html\r\nContent-Encoding: gzip", /* Extrem ekelhaft */
    reinterpret_cast<const char*>(index_html),
    index_html_len
  );
}

void handleData() {
  char dataJson[256];
  sprintf(dataJson, "{\"sensors\": {\"temperature\": %f, \"humidity\": %f, \"co2ppm\": %f}}", g_temperature, g_humidity, g_co2ppm);
  server.send(
    200,
    "application/json",
    dataJson
  );
}

void webserver_setup() {
  server.on("/", handleIndex);
  server.on("/data", handleData);
  server.begin();
  Serial.println("Webserver started");
}

void webserver_loop() {
  server.handleClient();
}
