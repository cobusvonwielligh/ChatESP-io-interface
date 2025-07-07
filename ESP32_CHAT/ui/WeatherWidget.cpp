#include "WeatherWidget.h"
#include "GuiTheme.h"

namespace UI {

static void anim_x_weather_cb(void * var, int32_t v)
{
    lv_obj_set_x((lv_obj_t *) var, v);
}

WeatherWidget::WeatherWidget() {}

lv_obj_t* WeatherWidget::create(lv_obj_t* parent) {
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_CONTENT, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};

    container = lv_obj_create(parent);
    lv_obj_remove_style_all(container);
    lv_obj_set_size(container, LV_PCT(100), LV_PCT(100));
    lv_obj_set_layout(container, LV_LAYOUT_GRID);
    lv_obj_set_style_grid_column_dsc_array(container, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(container, row_dsc, 0);
    lv_obj_add_style(container, &stylePanelVista, 0); // Updated: Use Vista panel style

    /* header */
    labelLocation = lv_label_create(container);
    lv_obj_set_style_text_font(labelLocation, &lv_font_montserrat_14, 0);
    lv_obj_set_grid_cell(labelLocation, LV_GRID_ALIGN_END, 2, 1,
                         LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_add_style(labelLocation, &widgetStyle, 0);

    iconLabel = lv_label_create(container);
    lv_obj_set_style_text_font(iconLabel, emojiFont, 0);
    lv_obj_set_grid_cell(iconLabel, LV_GRID_ALIGN_END, 1, 1,
                         LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_add_style(iconLabel, &widgetStyle, 0);

    /* temperature */
    labelTemp = lv_label_create(container);
    lv_obj_set_style_text_font(labelTemp, &lv_font_montserrat_48, 0);
    lv_obj_set_grid_cell(labelTemp, LV_GRID_ALIGN_START, 0, 1,
                         LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_add_style(labelTemp, &widgetStyle, 0);

    /* min/max */
    labelMinMax = lv_label_create(container);
    lv_obj_set_style_text_font(labelMinMax, &lv_font_montserrat_18, 0);
    lv_obj_set_grid_cell(labelMinMax, LV_GRID_ALIGN_START, 0, 2,
                         LV_GRID_ALIGN_START, 2, 1);
    lv_obj_add_style(labelMinMax, &widgetStyle, 0);

    /* progress */
    progressBar = lv_bar_create(container);
    lv_obj_set_grid_cell(progressBar, LV_GRID_ALIGN_STRETCH, 0, 3,
                         LV_GRID_ALIGN_START, 0, 1);
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
    snprintf(buf, sizeof(buf), "%.0f | %.0f", tempMin, tempMax);
    lv_label_set_text(labelMinMax, buf);
    lv_label_set_text(iconLabel, isRain ? "\xF0\x9F\x8C\xA7\xEF\xB8\x8F" : "\xE2\x98\x80\xEF\xB8\x8F"); // 🌧️ or ☀️
    lv_bar_set_value(progressBar, (int)(progress * 100), LV_ANIM_OFF);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, labelTemp);
    lv_anim_set_values(&a, -lv_obj_get_width(labelTemp), 0);
    lv_anim_set_time(&a, 500);
    lv_anim_set_exec_cb(&a, anim_x_weather_cb);
    lv_anim_set_path_cb(&a, lv_anim_path_overshoot);
    lv_anim_start(&a);
}

void WeatherWidget::setVisible(bool en) {
    if (!container) return;
    if (en) lv_obj_clear_flag(container, LV_OBJ_FLAG_HIDDEN);
    else lv_obj_add_flag(container, LV_OBJ_FLAG_HIDDEN);
}

} // namespace UI
