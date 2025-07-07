#include "GuiService.h"
#include "display.h"
#include "touch.h"
#include "ui/GuiTheme.h"
#include "ui/MainScreen.cpp" // new include

namespace UI {

  static lv_color_t* buf1 = nullptr;
  static lv_disp_draw_buf_t draw_buf;
  static lv_disp_drv_t disp_drv;
  static lv_indev_drv_t indev_drv;

  static void flush_cb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
      display.startWrite();
      display.setAddrWindow(area->x1, area->y1,
                            area->x2 - area->x1 + 1,
                            area->y2 - area->y1 + 1);
      display.pushPixels((uint16_t *)color_p,
                         (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1));
      display.endWrite();
      lv_disp_flush_ready(disp);
  }

  static void touch_cb(lv_indev_drv_t *drv, lv_indev_data_t *data) {
      int pos[2];
      readTouch(pos);
      if (pos[0] >= 0 && pos[1] >= 0) {
          data->state = LV_INDEV_STATE_PR;
          data->point.x = pos[0];
          data->point.y = pos[1];
      } else {
          data->state = LV_INDEV_STATE_REL;
      }
  }

  static UI::WeatherWidget weatherWidget;
  static UI::ChatWidget chatWidget;
  static bool ready = false;
  static lv_obj_t* mainScreen = nullptr;

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

      initTheme();

      mainScreen = UI::createMainScreen(NULL);
      // By default show weather widget
      weatherWidget.create(UI::getContentPanel());
      lv_scr_load(mainScreen);
      ready = true;
      return true;
  }

  void updateWeather(float t, float tMin, float tMax, bool isRain, float progress, const String &location) {
      if (!ready) return;
      weatherWidget.update(t, tMin, tMax, isRain, progress, location);
      // Optionally switch to weather page if not already visible
  }

  void showChat(const String &text) {
      if (!ready) return;
      chatWidget.create(UI::getContentPanel());
      chatWidget.setTextTypewriter(text);
      // Optionally switch to chat page if not already visible
  }

  void loop() {
      if (!ready) return;
      lv_timer_handler();
  }

  UI::WeatherWidget& weather() { return weatherWidget; }
  UI::ChatWidget& chat() { return chatWidget; }

}

