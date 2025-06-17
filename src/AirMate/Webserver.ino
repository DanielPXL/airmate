#include <WebServer.h>

WebServer server(80);

void handleIndex() {
  server.send(200, "text/html", "<html> <head></head> <body><h1>Hello World</h1></body> </html>");
}

void webserver_setup() {
  server.on("/", handleIndex);
  server.begin();
  Serial.println("Webserver started");
}

void webserver_loop() {
  server.handleClient();
}
