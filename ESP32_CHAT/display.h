#pragma once
#include <LovyanGFX.hpp>
#include "LGFX_ILI9488.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480

/* =========================================
 *        ILI9488 TFT Display Driver
 * ========================================= */


extern LGFX_ILI9488 display;

void initDisplay(LGFX_ILI9488& d);

void drawWeatherScreen(float tempC, float tempMin, float tempMax, bool isRain, float progress);
void drawLoadingAnimation();
void displayMessage(String message);
void drawChatGptScreen();
void drawBitmapImage(const uint8_t* bitmap, int width, int height);
