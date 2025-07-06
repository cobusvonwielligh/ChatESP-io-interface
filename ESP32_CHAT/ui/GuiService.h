#pragma once
#include <lvgl.h>
#include <Arduino.h>
#include "WeatherWidget.h"
#include "ChatWidget.h"

namespace ui {

bool begin();
void loop();
void updateWeather(float t, float tMin, float tMax, bool isRain, float progress, const String &location);
void showChat(const String &text);

WeatherWidget& weather();
ChatWidget& chat();

} // namespace ui
