#pragma once
#include <lvgl.h>
#include <Arduino.h>
#include "ui/WeatherWidget.h"
#include "ui/ChatWidget.h"

namespace UI {

bool begin();
void loop();
void updateWeather(float t, float tMin, float tMax, bool isRain, float progress, const String &location);
void showChat(const String &text);

WeatherWidget& weather();
ChatWidget& chat();

} // namespace UI
