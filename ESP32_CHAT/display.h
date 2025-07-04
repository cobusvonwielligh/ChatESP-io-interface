#pragma once
#define LGFX_MAKERFABS_TOUCHCAMERA
#include <LovyanGFX.hpp>
#include <lgfx/v1/fonts/FreeSansBold12pt7b.h>
#include "makerfabs_pin.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480

/* =========================================
 *        ILI9488 TFT Display Driver
 * ========================================= */


extern LGFX display;

void initDisplay(LGFX& d);

void drawWeatherScreen(float tempC, float tempMin, float tempMax, bool isRain, float progress);
void drawLoadingAnimation();
void displayMessage(String message);
void drawChatGptScreen();
void drawBitmapImage(const uint8_t* bitmap, int width, int height);
