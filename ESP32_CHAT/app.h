#pragma once

// Set DEBUG_MODE to 1 for a minimal build that only
// initializes the display for debugging.  Set to 0 to
// enable WiFi, weather updates, the ChatGPT client and
// the LVGL UI.
#define DEBUG_MODE 0

#include "display.h"
#include "chatgpt.h"
#include "weather.h"
#include "touch.h"
#include "audio.h"
#include "GuiService.h"
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
