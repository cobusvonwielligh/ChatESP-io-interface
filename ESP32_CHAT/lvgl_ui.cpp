#include "lvgl_ui.h"
#include "display.h"
#include "weather.h"
#include "weather_icons.h"

namespace lvgl_ui {
static bool ready = false;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf1 = nullptr;
static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;

static lv_obj_t *scr_weather;
static lv_obj_t *scr_chat;
static lv_obj_t *label_temp;
static lv_obj_t *label_location;
static lv_obj_t *label_min;
static lv_obj_t *label_max;
static lv_obj_t *btn_mic;
static lv_obj_t *img_icon;
static lv_obj_t *label_chat;
static lv_obj_t *progress_bar;
static lv_style_t style_bg;
static lv_obj_t *container_main;

static void create_header(lv_obj_t *parent);
static void create_temperature_section(lv_obj_t *parent);
static void create_minmax_section(lv_obj_t *parent);
static void create_footer(lv_obj_t *parent);

static const lv_image_dsc_t img_sun = {
    .header = {
        .magic = LV_IMAGE_HEADER_MAGIC,
        .cf = LV_COLOR_FORMAT_A1,
        .flags = 0,
        .w = 32,
        .h = 32,
        .stride = 4,
        .reserved_2 = 0
    },
    .data_size = sizeof(iconSunBitmap),
    .data = iconSunBitmap,
};

static const lv_image_dsc_t img_rain = {
    .header = {
        .magic = LV_IMAGE_HEADER_MAGIC,
        .cf = LV_COLOR_FORMAT_A1,
        .flags = 0,
        .w = 32,
        .h = 32,
        .stride = 4,
        .reserved_2 = 0
    },
    .data_size = sizeof(iconRainBitmap),
    .data = iconRainBitmap,
};

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

static void create_header(lv_obj_t *parent) {
  Serial.println("lvgl_ui: create_header");
  lv_obj_t *cont = lv_obj_create(parent);
  lv_obj_remove_style_all(cont);
  lv_obj_set_width(cont, LV_PCT(100));
  lv_obj_set_style_pad_all(cont, 4, 0);
  lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
  lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
  lv_obj_set_style_pad_column(cont, 8, 0);

  label_location = lv_label_create(cont);
  lv_obj_set_style_text_font(label_location, &lv_font_montserrat_16, 0);
  lv_obj_set_flex_grow(label_location, 1);

  img_icon = lv_image_create(cont);
}

static void create_temperature_section(lv_obj_t *parent) {
  Serial.println("lvgl_ui: create_temperature_section");
  lv_obj_t *cont = lv_obj_create(parent);
  lv_obj_remove_style_all(cont);
  lv_obj_set_width(cont, LV_PCT(100));
  lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);

  label_temp = lv_label_create(cont);
  lv_obj_set_style_text_font(label_temp, &lv_font_montserrat_48, 0);
  lv_obj_center(label_temp);
}

static void create_minmax_section(lv_obj_t *parent) {
  Serial.println("lvgl_ui: create_minmax_section");
  lv_obj_t *cont = lv_obj_create(parent);
  lv_obj_remove_style_all(cont);
  lv_obj_set_width(cont, LV_PCT(100));
  lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
  lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_pad_column(cont, 4, 0);

  label_min = lv_label_create(cont);
  lv_obj_set_style_text_font(label_min, &lv_font_montserrat_24, 0);

  label_max = lv_label_create(cont);
  lv_obj_set_style_text_font(label_max, &lv_font_montserrat_24, 0);
}

static void create_footer(lv_obj_t *parent) {
  Serial.println("lvgl_ui: create_footer");
  lv_obj_t *cont = lv_obj_create(parent);
  lv_obj_remove_style_all(cont);
  lv_obj_set_width(cont, LV_PCT(100));
  lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
  lv_obj_set_style_pad_all(cont, 4, 0);
  lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
  lv_obj_set_style_pad_column(cont, 8, 0);

  progress_bar = lv_bar_create(cont);
  lv_obj_set_flex_grow(progress_bar, 1);
  lv_obj_set_height(progress_bar, 8);

  btn_mic = lv_btn_create(cont);
  lv_obj_t *lbl = lv_label_create(btn_mic);
#ifdef LV_SYMBOL_MICROPHONE
  lv_label_set_text(lbl, LV_SYMBOL_MICROPHONE);
#else
  lv_label_set_text(lbl, "Mic");
#endif
}

bool begin() {
  Serial.println("lvgl_ui: begin");
  lv_init();

  uint32_t buf_size = SCREEN_WIDTH * 40;
  buf1 = (lv_color_t*)heap_caps_malloc(buf_size * sizeof(lv_color_t), MALLOC_CAP_DMA);
  if (!buf1) {
    Serial.println("LVGL buffer allocation failed");
    return false;
  }
  lv_disp_draw_buf_init(&draw_buf, buf1, NULL, buf_size);

  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = SCREEN_WIDTH;
  disp_drv.ver_res = SCREEN_HEIGHT;
  disp_drv.flush_cb = flush_cb;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);
  Serial.println("lvgl_ui: display driver registered");

  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = touch_cb;
  lv_indev_drv_register(&indev_drv);
  Serial.println("lvgl_ui: input driver registered");

  lv_style_init(&style_bg);
  lv_style_set_bg_color(&style_bg, lv_color_hex(0x316bb6));
  lv_style_set_bg_grad_color(&style_bg, lv_color_hex(0x000000));
  lv_style_set_bg_grad_dir(&style_bg, LV_GRAD_DIR_VER);

  scr_weather = lv_obj_create(NULL);
  lv_obj_remove_style_all(scr_weather);
  lv_obj_add_style(scr_weather, &style_bg, 0);
  lv_obj_set_size(scr_weather, SCREEN_WIDTH, SCREEN_HEIGHT);

  container_main = lv_obj_create(scr_weather);
  lv_obj_remove_style_all(container_main);
  lv_obj_set_size(container_main, LV_PCT(100), LV_PCT(100));
  lv_obj_set_flex_flow(container_main, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_pad_all(container_main, 8, 0);
  lv_obj_set_style_pad_row(container_main, 12, 0);
  lv_obj_set_style_bg_opa(container_main, LV_OPA_TRANSP, 0);

  create_header(container_main);
  create_temperature_section(container_main);
  create_minmax_section(container_main);
  create_footer(container_main);

  scr_chat = lv_obj_create(NULL);
  label_chat = lv_label_create(scr_chat);
  lv_label_set_long_mode(label_chat, LV_LABEL_LONG_WRAP);
  lv_obj_set_width(label_chat, SCREEN_WIDTH - 20);
  lv_obj_align(label_chat, LV_ALIGN_TOP_LEFT, 10, 10);

  lv_scr_load(scr_weather);
  ready = true;
  Serial.println("lvgl_ui: ready");
  return true;
}

void loop() {
  if (!ready) return;
  lv_timer_handler();
}

void updateWeather(float tempC, float tempMin, float tempMax, bool isRain, float progress) {
  if (!ready) return;
  char buf[64];
  lv_label_set_text(label_location, LOCATION_NAME.c_str());
  snprintf(buf, sizeof(buf), "%.1f%cC", tempC, 0xB0);
  lv_label_set_text(label_temp, buf);
  snprintf(buf, sizeof(buf), "Min: %.0f%cC", tempMin, 0xB0);
  lv_label_set_text(label_min, buf);
  snprintf(buf, sizeof(buf), "Max: %.0f%cC", tempMax, 0xB0);
  lv_label_set_text(label_max, buf);
  lv_image_set_src(img_icon, isRain ? &img_rain : &img_sun);
  lv_bar_set_value(progress_bar, (int)(progress * 100), LV_ANIM_OFF);
  lv_scr_load(scr_weather);
}

void showChat(const String &text) {
  if (!ready) return;
  lv_label_set_text(label_chat, text.c_str());
  lv_scr_load(scr_chat);
}

} // namespace lvgl_ui
