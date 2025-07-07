#include "GuiTheme.h"
#include <lvgl.h>

// Menu bar uses simple arrow symbols rather than image assets

namespace UI {

static lv_obj_t* menuBar = nullptr;
static lv_obj_t* contentPanel = nullptr;

void updateMenuBarIcons() {
    if (!menuBar) return;
    lv_obj_clean(menuBar);

    lv_obj_t* btn_left = lv_btn_create(menuBar);
    lv_obj_add_style(btn_left, &styleBtnVista, 0);
    lv_obj_add_style(btn_left, &styleBtnVistaPressed, LV_STATE_PRESSED);
    lv_obj_set_size(btn_left, 36, 36);
    lv_obj_align(btn_left, LV_ALIGN_LEFT_MID, 8, 0);
    lv_obj_t* label_left = lv_label_create(btn_left);
    lv_label_set_text(label_left, LV_SYMBOL_LEFT);

    lv_obj_t* btn_right = lv_btn_create(menuBar);
    lv_obj_add_style(btn_right, &styleBtnVista, 0);
    lv_obj_add_style(btn_right, &styleBtnVistaPressed, LV_STATE_PRESSED);
    lv_obj_set_size(btn_right, 36, 36);
    lv_obj_align(btn_right, LV_ALIGN_RIGHT_MID, -8, 0);
    lv_obj_t* label_right = lv_label_create(btn_right);
    lv_label_set_text(label_right, LV_SYMBOL_RIGHT);
}

lv_obj_t* createMainScreen(lv_obj_t* parent) {
    static lv_coord_t row_dsc[] = {LV_GRID_FR(8), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_obj_t* main_grid = lv_obj_create(parent);
    lv_obj_set_size(main_grid, LV_PCT(100), LV_PCT(100));
    lv_obj_center(main_grid);
    lv_obj_set_layout(main_grid, LV_LAYOUT_GRID);
    lv_obj_set_style_grid_column_dsc_array(main_grid, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(main_grid, row_dsc, 0);
    lv_obj_add_style(main_grid, &stylePanelVista, 0);

    // Content panel (for WeatherWidget or ChatWidget)
    contentPanel = lv_obj_create(main_grid);
    lv_obj_set_grid_cell(contentPanel, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_add_style(contentPanel, &stylePanelVista, 0);

    // Bottom menu bar
    menuBar = lv_obj_create(main_grid);
    lv_obj_set_grid_cell(menuBar, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_obj_set_height(menuBar, 40);
    lv_obj_add_style(menuBar, &styleMenuBar, 0);

    // Add a loading/progress bar to menu bar
    lv_obj_t* bar = lv_bar_create(menuBar);
    lv_obj_set_size(bar, 120, 8);
    lv_obj_align(bar, LV_ALIGN_BOTTOM_MID, 0, -6);
    lv_bar_set_value(bar, 60, LV_ANIM_ON);

    updateMenuBarIcons();

    return main_grid;
}

lv_obj_t* getContentPanel() { return contentPanel; }
lv_obj_t* getMenuBar() { return menuBar; }

}

