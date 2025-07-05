#pragma once
#include <Arduino.h>
/* Ensure our local configuration is picked up by lvgl */
#ifndef LV_CONF_INCLUDE_SIMPLE
#define LV_CONF_INCLUDE_SIMPLE
#endif

#include "lv_conf.h"
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
