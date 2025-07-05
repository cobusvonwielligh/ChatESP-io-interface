#include <WiFi.h>
#include <Wire.h>

#include "secrets.h"
#include "app.h"

void setup() {
  app::begin();
}

void loop() {
  app::loop();
  delay(100);
}