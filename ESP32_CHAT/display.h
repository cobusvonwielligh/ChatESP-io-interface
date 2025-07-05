#pragma once

#define LGFX_MAKERFABS_TOUCHCAMERA
#ifndef LGFX_USE_V1
#define LGFX_USE_V1
#endif

#include "makerfabs_pin.h"
#include "LGFX_ILI9488.h"

// Only define LGFX alias if not already defined by LovyanGFX
#if !defined(__LGFX_DEF) && !defined(LovyanGFX_DEFINED_LGFX)
  using LGFX = LGFX_ILI9488;
#endif

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480

extern LGFX display;

void initDisplay(LGFX& d);
void drawWeatherScreen(float tempC, float tempMin, float tempMax, bool isRain, float progress);
void drawLoadingAnimation();
void displayMessage(String message);
void drawChatGptScreen();
void drawBitmapImage(const uint8_t* bitmap, int width, int height);
