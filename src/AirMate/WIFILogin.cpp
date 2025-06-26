#include "WIFILogin.h"
#include <WiFi.h>
#include <HardwareSerial.h>

const char WIFI_SSID[] = "SSID_HERE";
const char WIFI_PASSWORD[] = "PASSWORD_HERE";

void wifi_connect() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.setAutoReconnect(true);
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());
}
