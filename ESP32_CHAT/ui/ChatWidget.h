#pragma once
#include <lvgl.h>
#include <Arduino.h>

namespace UI {

class ChatWidget {
public:
    ChatWidget();
    lv_obj_t* create(lv_obj_t* parent);
    void setText(const String &text);
    void setTextTypewriter(const String &text);
    lv_obj_t* root() const { return container; }
    friend void typewriter_cb(lv_timer_t* timer);
private:
    lv_obj_t* container = nullptr;
    lv_obj_t* label = nullptr;
};

} // namespace UI