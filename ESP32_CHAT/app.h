#pragma once

// Define DEBUG_MODE to disable most features and only
// initialize the display for debugging.
#define DEBUG_MODE 1

#include "display.h"
#include "chatgpt.h"
#include "weather.h"
#include "touch.h"
#include "audio.h"
#include "lvgl_ui.h"
#include <WiFi.h>
#include <Wire.h>

namespace app {

enum class Page { Weather, ChatGpt };

struct State {
  Page page;
  float tempC;
  float tempMin;
  float tempMax;
  uint8_t weatherCode;
  bool raining;
  unsigned long lastWeather;
  int weatherFail;
};

void begin();
void loop();

} // namespace app
