#pragma once
#include <Arduino.h>
/*
 * Use the shared LVGL configuration stored in the Arduino libraries
 * folder.  The build defines LV_CONF_PATH so lvgl picks up this file
 * automatically.  Adjust the relative path if your setup differs.
 */
#ifndef LV_CONF_PATH
#define LV_CONF_PATH "../../../libraries/lv_conf.h"
#endif
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
