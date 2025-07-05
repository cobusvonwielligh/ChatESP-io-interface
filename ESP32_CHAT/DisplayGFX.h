#pragma once
#ifndef LGFX_USE_V1
#define LGFX_USE_V1
#endif
#include <LovyanGFX.hpp>
#include "makerfabs_pin.h"

class DisplayGFX : public lgfx::LGFX_Device {
  lgfx::Bus_SPI           _bus_instance;
  lgfx::Panel_ILI9488     _panel_instance;
#if defined(FT6236_TOUCH)
  lgfx::Touch_FT5x06      _touch_instance;
#endif

public:
  DisplayGFX(void) {
    {
      auto cfg = _bus_instance.config();
      cfg.spi_host = LCD_SPI_HOST;
      cfg.spi_mode = 0;
      cfg.freq_write = 60000000;
      cfg.freq_read  = 16000000;
      cfg.spi_3wire  = false;
      cfg.use_lock   = true;
      cfg.dma_channel = SPI_DMA_CH_AUTO;
      cfg.pin_sclk = LCD_SCK;
      cfg.pin_mosi = LCD_MOSI;
      cfg.pin_miso = LCD_MISO;
      cfg.pin_dc   = LCD_DC;
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs   = LCD_CS;
      cfg.pin_rst  = LCD_RST;
      cfg.pin_busy = -1;
      cfg.panel_width      = LCD_WIDTH;
      cfg.panel_height     = LCD_HEIGHT;
      cfg.memory_width     = LCD_WIDTH;
      cfg.memory_height    = LCD_HEIGHT;
      cfg.offset_x         = 0;
      cfg.offset_y         = 0;
      cfg.offset_rotation  = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits  = 1;
      cfg.readable         = true;
      cfg.invert           = false;
      cfg.rgb_order        = false;
      cfg.dlen_16bit       = false;
      cfg.bus_shared       = true;
      _panel_instance.config(cfg);
    }

#if defined(FT6236_TOUCH)
    {
      auto cfg = _touch_instance.config();
      cfg.x_min      = 0;
      cfg.x_max      = LCD_WIDTH  - 1;
      cfg.y_min      = 0;
      cfg.y_max      = LCD_HEIGHT - 1;
      cfg.bus_shared = true;
      cfg.offset_rotation = 0;
      cfg.i2c_port = 1;
      cfg.i2c_addr = TOUCH_I2C_ADD;
      cfg.pin_sda  = I2C_SDA;
      cfg.pin_scl  = I2C_SCL;
      cfg.freq = 400000;
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }
#endif

    setPanel(&_panel_instance);
  }
};
