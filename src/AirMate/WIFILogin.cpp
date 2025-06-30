#include "WIFILogin.h"
#include <WiFi.h>
#include "Log.h"

const char WIFI_SSID[] = "SSID_HERE";
const char WIFI_PASSWORD[] = "PASSWORD_HERE";

void wifi_connect() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.setAutoReconnect(true);
  LOG("Connecting to %s", WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    LOG(".");
  }

  LOG("\nConnected. IP: %s\n", WiFi.localIP().toString());
}
