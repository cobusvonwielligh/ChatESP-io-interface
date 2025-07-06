#include "WeatherWidget.h"
#include "../weather_icons.h"

extern "C" {
extern const lv_img_dsc_t icon_sun;
extern const lv_img_dsc_t icon_rain;
}

namespace UI {

WeatherWidget::WeatherWidget() {}

lv_obj_t* WeatherWidget::create(lv_obj_t* parent) {
    container = lv_obj_create(parent);
    lv_obj_remove_style_all(container);
    lv_obj_set_size(container, LV_PCT(100), LV_PCT(100));
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(container, 6, 0);
    lv_obj_set_style_pad_row(container, 14, 0);
    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, 0);

    // header with location and icon
    lv_obj_t* header = lv_obj_create(container);
    lv_obj_remove_style_all(header);
    lv_obj_set_width(header, LV_PCT(100));
    lv_obj_set_style_pad_all(header, 4, 0);
    lv_obj_set_style_bg_opa(header, LV_OPA_TRANSP, 0);
    lv_obj_set_flex_flow(header, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_column(header, 8, 0);

    labelLocation = lv_label_create(header);
    lv_obj_set_style_text_font(labelLocation, &lv_font_montserrat_14, 0);
    lv_obj_set_flex_grow(labelLocation, 1);

    imgIcon = lv_img_create(header);

    // temperature section
    lv_obj_t* tempCont = lv_obj_create(container);
    lv_obj_remove_style_all(tempCont);
    lv_obj_set_width(tempCont, LV_PCT(100));
    lv_obj_set_style_bg_opa(tempCont, LV_OPA_TRANSP, 0);

    labelTemp = lv_label_create(tempCont);
    lv_obj_set_style_text_font(labelTemp, &lv_font_montserrat_48, 0);
    lv_obj_center(labelTemp);

    // min/max section
    lv_obj_t* minmaxCont = lv_obj_create(container);
    lv_obj_remove_style_all(minmaxCont);
    lv_obj_set_width(minmaxCont, LV_PCT(100));
    lv_obj_set_style_bg_opa(minmaxCont, LV_OPA_TRANSP, 0);
    lv_obj_set_flex_flow(minmaxCont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_column(minmaxCont, 4, 0);

    labelMin = lv_label_create(minmaxCont);
    lv_obj_set_style_text_font(labelMin, &lv_font_montserrat_18, 0);

    labelMax = lv_label_create(minmaxCont);
    lv_obj_set_style_text_font(labelMax, &lv_font_montserrat_18, 0);

    // footer with progress bar
    lv_obj_t* footer = lv_obj_create(container);
    lv_obj_remove_style_all(footer);
    lv_obj_set_width(footer, LV_PCT(100));
    lv_obj_set_style_bg_opa(footer, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_all(footer, 4, 0);
    lv_obj_set_flex_flow(footer, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_column(footer, 8, 0);

    progressBar = lv_bar_create(footer);
    lv_obj_set_flex_grow(progressBar, 1);
    lv_obj_set_height(progressBar, 6);
    lv_obj_set_style_bg_color(progressBar, lv_palette_darken(LV_PALETTE_GREY, 3), LV_PART_MAIN);
    lv_obj_set_style_bg_color(progressBar, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);

    return container;
}

void WeatherWidget::update(float tempC, float tempMin, float tempMax, bool isRain, float progress, const String &location) {
    if (!container) return;
    char buf[64];
    lv_label_set_text(labelLocation, location.c_str());
    snprintf(buf, sizeof(buf), "%.1f%cC", tempC, 0xB0);
    lv_label_set_text(labelTemp, buf);
    snprintf(buf, sizeof(buf), "Min: %.0f%cC", tempMin, 0xB0);
    lv_label_set_text(labelMin, buf);
    snprintf(buf, sizeof(buf), "Max: %.0f%cC", tempMax, 0xB0);
    lv_label_set_text(labelMax, buf);
    lv_img_set_src(imgIcon, isRain ? &icon_rain : &icon_sun);
    lv_bar_set_value(progressBar, (int)(progress * 100), LV_ANIM_OFF);
}

} // namespace UI
