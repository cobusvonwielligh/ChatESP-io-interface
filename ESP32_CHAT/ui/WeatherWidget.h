#pragma once
#include <lvgl.h>
#include <Arduino.h>

namespace UI {

class WeatherWidget {
public:
    WeatherWidget();
    lv_obj_t* create(lv_obj_t* parent);
    void update(float tempC, float tempMin, float tempMax, bool isRain, float progress, const String &location);
    void setVisible(bool en);
    lv_obj_t* root() const { return container; }
private:
    lv_obj_t* container = nullptr;
    lv_obj_t* labelTemp = nullptr;
    lv_obj_t* labelLocation = nullptr;
    lv_obj_t* labelMin = nullptr;
    lv_obj_t* labelMax = nullptr;
    lv_obj_t* iconLabel = nullptr;
    lv_obj_t* progressBar = nullptr;
};

} // namespace UI
