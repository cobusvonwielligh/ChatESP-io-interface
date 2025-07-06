#pragma once
#include <lvgl.h>

namespace UI {

extern lv_style_t stylePanelVista;
extern lv_style_t styleMenuBar;
extern lv_style_t styleBtnVista;
extern lv_style_t styleBtnVistaPressed;
extern lv_style_t widgetStyle;    // Legacy, keep for widget usage
extern const lv_font_t *emojiFont;

void initTheme();

}