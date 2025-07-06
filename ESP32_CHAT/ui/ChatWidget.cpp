#include "ChatWidget.h"

namespace ui {

ChatWidget::ChatWidget() {}

lv_obj_t* ChatWidget::create(lv_obj_t* parent) {
    container = lv_obj_create(parent);
    lv_obj_set_size(container, LV_PCT(100), LV_PCT(100));
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_ACTIVE);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(container, 10, 0);

    label = lv_label_create(container);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(label, LV_PCT(100));
    return container;
}

void ChatWidget::setText(const String &text) {
    if (!label) return;
    lv_label_set_text(label, text.c_str());
}

} // namespace ui
