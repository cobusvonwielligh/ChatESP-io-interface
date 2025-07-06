#include "GuiService.h"
#include "display.h"
#include "touch.h"
#include "ui/GuiTheme.h"
#include "ui/MainScreen.cpp" // new include

namespace UI {

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

  void setMenuIcons(lv_img_dsc_t* left, lv_img_dsc_t* right) {
      UI::updateMenuBarIcons(left, right);
  }

}
