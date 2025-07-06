#include "ui/GuiService.h"
#include "display.h"
#include "touch.h"

namespace ui {

static WeatherWidget weatherWidget;
static ChatWidget chatWidget;
static bool ready = false;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf1 = nullptr;
static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;
static lv_obj_t *scr_weather = nullptr;
static lv_obj_t *scr_chat = nullptr;

static void flush_cb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;
    display.startWrite();
    display.setAddrWindow(area->x1, area->y1, w, h);
    display.pushColors((uint16_t *)&color_p->full, w * h, true);
    display.endWrite();
    lv_disp_flush_ready(disp);
}

static void touch_cb(lv_indev_drv_t *drv, lv_indev_data_t *data) {
    int pos[2] = { -1, -1 };
    readTouch(pos);
    if (pos[0] < 0 || pos[1] < 0) {
        data->state = LV_INDEV_STATE_REL;
    } else {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = pos[0];
        data->point.y = pos[1];
    }
}

bool begin() {
    lv_init();
    uint32_t buf_size = SCREEN_WIDTH * 40;
    buf1 = (lv_color_t*)heap_caps_malloc(buf_size * sizeof(lv_color_t), MALLOC_CAP_DMA);
    if (!buf1) {
        return false;
    }
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, buf_size);

    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;
    disp_drv.flush_cb = flush_cb;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touch_cb;
    lv_indev_drv_register(&indev_drv);

    lv_theme_t *theme = lv_theme_default_init(lv_disp_get_default(),
                                              lv_palette_main(LV_PALETTE_BLUE),
                                              lv_palette_main(LV_PALETTE_RED),
                                              LV_THEME_DEFAULT_DARK,
                                              LV_FONT_DEFAULT);
    lv_disp_set_theme(lv_disp_get_default(), theme);

    scr_weather = lv_obj_create(NULL);
    lv_obj_remove_style_all(scr_weather);
    weatherWidget.create(scr_weather);

    scr_chat = lv_obj_create(NULL);
    lv_obj_remove_style_all(scr_chat);
    chatWidget.create(scr_chat);

    lv_scr_load(scr_weather);
    ready = true;
    return true;
}

void loop() {
    if (!ready) return;
    lv_timer_handler();
}

void updateWeather(float t, float tMin, float tMax, bool isRain, float progress, const String &location) {
    if (!ready) return;
    weatherWidget.update(t, tMin, tMax, isRain, progress, location);
    lv_scr_load(scr_weather);
}

void showChat(const String &text) {
    if (!ready) return;
    chatWidget.setText(text);
    lv_scr_load(scr_chat);
}

WeatherWidget& weather() { return weatherWidget; }
ChatWidget& chat() { return chatWidget; }

} // namespace ui
