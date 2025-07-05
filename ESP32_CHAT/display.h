#pragma once
#define LGFX_MAKERFABS_TOUCHCAMERA
#ifndef LGFX_USE_V1
#define LGFX_USE_V1
#endif
#include <LovyanGFX.hpp>
#include "makerfabs_pin.h"
#include "DisplayGFX.h"

// Display driver configured for the Makerfabs 3.5" ILI9488 touch screen.
// The concrete class is named `DisplayGFX` and exposed project‑wide.

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
