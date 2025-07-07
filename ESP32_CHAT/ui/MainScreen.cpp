#include "GuiTheme.h"
#include <lvgl.h>

namespace UI {

#include "../assets/imgbtn_left.c"
#include "../assets/imgbtn_right.c"

static lv_obj_t* menuBar = nullptr;
static lv_obj_t* contentPanel = nullptr;

enum class Page { Weather, ChatGpt };
typedef void (*PageChangeCb)(Page);
static PageChangeCb pageChangeCb = nullptr;
static Page currentPage = Page::Weather;

static void btn_left_cb(lv_event_t * e) {
    if(pageChangeCb) pageChangeCb(Page::Weather);
}

static void btn_right_cb(lv_event_t * e) {
    if(pageChangeCb) pageChangeCb(Page::ChatGpt);
}

static void rebuildMenuBar() {
    if (!menuBar) return;
    lv_obj_clean(menuBar);

    lv_obj_t* bar = lv_bar_create(menuBar);
    lv_obj_set_size(bar, 120, 8);
    lv_obj_align(bar, LV_ALIGN_BOTTOM_MID, 0, -6);
    lv_bar_set_value(bar, 60, LV_ANIM_ON);

    if(currentPage == Page::Weather) {
        lv_obj_t* btn = lv_btn_create(menuBar);
        lv_obj_add_style(btn, &styleBtnVista, 0);
        lv_obj_add_style(btn, &styleBtnVistaPressed, LV_STATE_PRESSED);
        lv_obj_set_size(btn, 36, 36);
        lv_obj_align(btn, LV_ALIGN_RIGHT_MID, -10, 0);
        lv_obj_add_event_cb(btn, btn_right_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_t* img = lv_img_create(btn);
        LV_IMG_DECLARE(imagebutton_right);
        lv_img_set_src(img, &imagebutton_right);
    } else {
        lv_obj_t* btn = lv_btn_create(menuBar);
        lv_obj_add_style(btn, &styleBtnVista, 0);
        lv_obj_add_style(btn, &styleBtnVistaPressed, LV_STATE_PRESSED);
        lv_obj_set_size(btn, 36, 36);
        lv_obj_align(btn, LV_ALIGN_LEFT_MID, 10, 0);
        lv_obj_add_event_cb(btn, btn_left_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_t* img = lv_img_create(btn);
        LV_IMG_DECLARE(imagebutton_left);
        lv_img_set_src(img, &imagebutton_left);
    }
}

void setPage(Page p) {
    currentPage = p;
    rebuildMenuBar();
}

lv_obj_t* createMainScreen(lv_obj_t* parent, PageChangeCb cb) {
    pageChangeCb = cb;
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

    rebuildMenuBar();

    return main_grid;
}

lv_obj_t* getContentPanel() { return contentPanel; }
lv_obj_t* getMenuBar() { return menuBar; }

}

