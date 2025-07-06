#include "ChatWidget.h"
#include "GuiTheme.h"

namespace UI {

static void anim_x_chat_cb(void * var, int32_t v)
{
    lv_obj_set_x((lv_obj_t *) var, v);
}

ChatWidget::ChatWidget() {}

lv_obj_t* ChatWidget::create(lv_obj_t* parent) {
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};

    container = lv_obj_create(parent);
    lv_obj_set_size(container, LV_PCT(100), LV_PCT(100));
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_ACTIVE);
    lv_obj_set_layout(container, LV_LAYOUT_GRID);
    lv_obj_set_style_grid_column_dsc_array(container, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(container, row_dsc, 0);
    lv_obj_add_style(container, &widgetStyle, 0);

    label = lv_label_create(container);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_START, 0, 1);
    lv_obj_set_width(label, LV_PCT(100));
    lv_obj_add_style(label, &widgetStyle, 0);
    return container;
}

void ChatWidget::setText(const String &text) {
    if (!label) return;
    lv_label_set_text(label, text.c_str());

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, label);
    lv_anim_set_values(&a, -lv_obj_get_width(label), 0);
    lv_anim_set_time(&a, 500);
    lv_anim_set_exec_cb(&a, anim_x_chat_cb);
    lv_anim_set_path_cb(&a, lv_anim_path_overshoot);
    lv_anim_start(&a);
}

} // namespace UI
