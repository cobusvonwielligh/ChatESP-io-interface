#include "lvgl_ui.h"
#include "display.h"

using namespace lvgl::widget;

namespace lvgl_ui {
static bool ready = false;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf1 = nullptr;
static lv_color_t *buf2 = nullptr;
static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;

static Label *label_temp;
static Label *label_chat;
static Bar *progress_bar;

static lv_obj_t *scr_weather;
static lv_obj_t *scr_chat;

static void flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p) {
#ifdef USE_TFT_ESPI
  display.startWrite();
  display.pushImage(area->x1, area->y1,
                    area->x2 - area->x1 + 1,
                    area->y2 - area->y1 + 1,
                    (uint16_t *)color_p);
  display.endWrite();
#else
  display.startWrite();
  display.pushImage(area->x1, area->y1,
                    area->x2 - area->x1 + 1,
                    area->y2 - area->y1 + 1,
                    (lgfx::rgb565_t *)color_p);
  display.endWrite();
#endif
  lv_disp_flush_ready(drv);
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

static void createWeatherScreen() {
  scr_weather = lv_obj_create(NULL);
  label_temp = new Label(scr_weather);
  label_temp->SetAlign(LV_ALIGN_TOP_MID, 0, 20);
  progress_bar = new Bar(scr_weather);
  progress_bar->SetRange(0, 100);
  progress_bar->SetSize(SCREEN_WIDTH - 20, 8);
  progress_bar->SetAlign(LV_ALIGN_BOTTOM_MID, 0, -10);
}

static void createChatScreen() {
  scr_chat = lv_obj_create(NULL);
  label_chat = new Label(scr_chat);
  label_chat->SetLongMode(LV_LABEL_LONG_WRAP);
  label_chat->SetWidth(SCREEN_WIDTH - 20);
  label_chat->SetAlign(LV_ALIGN_TOP_LEFT, 10, 10);
}

bool begin() {
  Serial.println("lvgl_ui: begin");
  lv_init();

  uint32_t buf_size = SCREEN_WIDTH * 40;
  buf1 = (lv_color_t*)heap_caps_malloc(buf_size * sizeof(lv_color_t), MALLOC_CAP_DMA);
  buf2 = (lv_color_t*)heap_caps_malloc(buf_size * sizeof(lv_color_t), MALLOC_CAP_DMA);
  if (!buf1 || !buf2) {
    Serial.println("LVGL buffer allocation failed");
    return false;
  }
  lv_disp_draw_buf_init(&draw_buf, buf1, buf2, buf_size);

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

  createWeatherScreen();
  createChatScreen();

  lv_scr_load(scr_weather);
  ready = true;
  Serial.println("lvgl_ui: ready");
  return true;
}

void loop() {
  if (!ready) return;
  lv_timer_handler();
}

void updateWeather(float tempC, float tempMin, float tempMax, bool isRain,
                   float progress) {
  label_temp->SetText("%.1f C (%.0f/%.0f)", tempC, tempMin, tempMax);
  progress_bar->SetValue((int)(progress * 100), LV_ANIM_OFF);

  if (ready) lv_scr_load(scr_weather);
}

void showChat(const String &text) {
  if (!ready) return;
  label_chat->SetText("%s", text.c_str());
  lv_scr_load(scr_chat);
}

} // namespace lvgl_ui
