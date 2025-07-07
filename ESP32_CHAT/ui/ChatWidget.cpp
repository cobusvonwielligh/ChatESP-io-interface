#include "ChatWidget.h"
#include "GuiTheme.h"

namespace UI {

extern "C" {
    LV_IMG_DECLARE(img_hand);
}

static lv_timer_t* typewriterTimer = nullptr;
static String typewriterText;
static size_t typewriterPos = 0;
static ChatWidget* activeWidget = nullptr;

void typewriter_cb(lv_timer_t* timer) {
    if (!activeWidget) return;
    if (typewriterPos < typewriterText.length()) {
        lv_label_set_text(activeWidget->label, typewriterText.substring(0, typewriterPos + 1).c_str());
        typewriterPos++;
    } else {
        lv_timer_del(typewriterTimer);
        typewriterTimer = nullptr;
    }
}

ChatWidget::ChatWidget() {}

lv_obj_t* ChatWidget::create(lv_obj_t* parent) {
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};

    container = lv_obj_create(parent);
    lv_obj_set_size(container, LV_PCT(100), LV_PCT(100));
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_ACTIVE);
    lv_obj_set_layout(container, LV_LAYOUT_GRID);
    lv_obj_set_style_grid_column_dsc_array(container, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(container, row_dsc, 0);
    lv_obj_add_style(container, &stylePanelVista, 0);

    icon = lv_img_create(container);
    lv_img_set_src(icon, &img_hand);
    lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_CENTER, 0, 1,
                         LV_GRID_ALIGN_START, 0, 1);

    label = lv_label_create(container);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_START, 1, 1);
    lv_obj_set_width(label, LV_PCT(100));
    lv_obj_add_style(label, &widgetStyle, 0);
    return container;
}

void ChatWidget::setTextTypewriter(const String &text) {
    if (!label) return;
    typewriterText = text;
    typewriterPos = 0;
    activeWidget = this;
    lv_label_set_text(label, "");
    if (typewriterTimer) lv_timer_del(typewriterTimer);
    typewriterTimer = lv_timer_create(typewriter_cb, 15, nullptr);
}

void ChatWidget::setText(const String &text) {
    // Fallback: instant set
    if (!label) return;
    lv_label_set_text(label, text.c_str());
}

void ChatWidget::setVisible(bool en) {
    if (!container) return;
    if (en) lv_obj_clear_flag(container, LV_OBJ_FLAG_HIDDEN);
    else lv_obj_add_flag(container, LV_OBJ_FLAG_HIDDEN);
}

} // namespace UI
