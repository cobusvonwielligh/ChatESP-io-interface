#pragma once
#include <LovyanGFX.hpp>
#include "makerfabs_pin.h"

class LGFX : public lgfx::LGFX_Device {
    lgfx::Panel_ILI9488 _panel;
    lgfx::Bus_SPI _bus;
public:
    LGFX() {
        {
            auto cfg = _bus.config();
            cfg.spi_host = LCD_SPI_HOST;
            cfg.spi_mode = 0;
            cfg.freq_write = 60000000;
            cfg.freq_read = 16000000;
            cfg.spi_3wire = false;
            cfg.use_lock = true;
            cfg.dma_channel = 1;
            cfg.pin_sclk = LCD_SCK;
            cfg.pin_mosi = LCD_MOSI;
            cfg.pin_miso = LCD_MISO;
            cfg.pin_dc   = LCD_DC;
            _bus.config(cfg);
            _panel.setBus(&_bus);
        }
        {
            auto cfg = _panel.config();
            cfg.pin_cs   = LCD_CS;
            cfg.pin_rst  = LCD_RST;
            cfg.pin_busy = -1;
            cfg.memory_width  = LCD_WIDTH;
            cfg.memory_height = LCD_HEIGHT;
            cfg.panel_width   = LCD_WIDTH;
            cfg.panel_height  = LCD_HEIGHT;
            cfg.offset_x = 0;
            cfg.offset_y = 0;
            cfg.offset_rotation = 0;
            cfg.dummy_read_pixel = 8;
            cfg.dummy_read_bits  = 1;
            cfg.readable = true;
            cfg.invert = false;
            cfg.rgb_order = false;
            cfg.dlen_16bit = false;
            cfg.bus_shared = true;
            _panel.config(cfg);
        }
        setPanel(&_panel);
    }
};
