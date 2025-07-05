#pragma once
#define LGFX_MAKERFABS_TOUCHCAMERA
#ifndef LGFX_USE_V1
#define LGFX_USE_V1
#endif
#include <LovyanGFX.hpp>
#include "makerfabs_pin.h"
#include "LGFX_ILI9488.h"

// Use the custom ILI9488 configuration from LGFX without clashing
// with the built-in `LGFX` alias that LovyanGFX may define.  The
// display type is exposed as `DisplayGFX` throughout the project.
using DisplayGFX = LGFX_ILI9488;

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480

/* =========================================
 *        ILI9488 TFT Display Driver
 * ========================================= */


extern DisplayGFX display;

void initDisplay(DisplayGFX& d);
void drawWeatherScreen(float tempC, float tempMin, float tempMax, bool isRain, float progress);
void drawLoadingAnimation();
void displayMessage(String message);
void drawChatGptScreen();
void drawBitmapImage(const uint8_t* bitmap, int width, int height);
