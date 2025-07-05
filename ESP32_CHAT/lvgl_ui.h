#pragma once
#include <Arduino.h>
#include <lvglCpp.h>
#include "display.h"
#include "touch.h"

namespace lvgl_ui {
  void begin();
  void loop();
  void updateWeather(float tempC, float tempMin, float tempMax, bool isRain,
                     float progress);
  void showChat(const String &text);
}
