void setup() {
  Serial.begin(9600);
  wifi_connect();
  webserver_setup();
  window_setup();
  sensors_setup();
  weather_setup();
}

void loop() {
  static int64_t lastUpdate = 0;
  int64_t now = millis();
  if (now - lastUpdate > 10000) {
    lastUpdate = now;

    weather_update();
    sensors_update();
  }

  window_loop();
  webserver_loop();
}