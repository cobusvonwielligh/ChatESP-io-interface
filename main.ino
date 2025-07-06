#include <lvgl.h>
#include <TFT_eSPI.h>
#include <Wire.h>

TFT_eSPI tft = TFT_eSPI();
lv_disp_draw_buf_t draw_buf;
lv_color_t buf[LV_HOR_RES_MAX * 10];

#define TOUCH_I2C_SDA 21
#define TOUCH_I2C_SCL 22
#define TOUCH_INT     -1  // not used
#define TOUCH_RST     -1  // not used

#include <FT6236.h>
FT6236 ts = FT6236();

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = area->x2 - area->x1 + 1;
  uint32_t h = area->y2 - area->y1 + 1;

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

void my_touch_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data) {
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    data->state = LV_INDEV_STATE_PR;
    data->point.x = p.x;
    data->point.y = p.y;
  } else {
    data->state = LV_INDEV_STATE_REL;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting...");

  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  Wire.begin(TOUCH_I2C_SDA, TOUCH_I2C_SCL);
  if (!ts.begin(40)) {
    Serial.println("Touch not found");
  }

  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, LV_HOR_RES_MAX * 10);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 320;
  disp_drv.ver_res = 480;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touch_read;
  lv_indev_drv_register(&indev_drv);

  // Simple label to test everything
  lv_obj_t *label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "✅ Screen + LVGL + Touch working!");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}

void loop() {
  lv_timer_handler(); 
  delay(5);
}
