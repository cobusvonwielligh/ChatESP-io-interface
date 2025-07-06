#pragma once
#include "makerfabs_pin.h"
#ifndef USE_TFT_ESPI
#define USE_TFT_ESPI
#endif

/*
 * Select the underlying display driver. By default the code used the
 * LovyanGFX based `DisplayGFX` class. The Makerfabs demo uses the
 * `TFT_eSPI` library so a compile time flag can switch between the two
 * implementations. Defining `USE_TFT_ESPI` will use the TFT_eSPI
 * driver instead of LovyanGFX.
 */

#ifdef USE_TFT_ESPI
#  include <TFT_eSPI.h>
#else
#  define LGFX_MAKERFABS_TOUCHCAMERA
#  ifndef LGFX_USE_V1
#    define LGFX_USE_V1
#  endif
#  include <LovyanGFX.hpp>
#  include "DisplayGFX.h"
#endif

// Display driver configured for the Makerfabs 3.5" ILI9488 touch screen.
// The concrete class is named `DisplayGFX` and exposed project‑wide.

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480

/* =========================================
 *        ILI9488 TFT Display Driver
 * ========================================= */


#ifdef USE_TFT_ESPI
extern TFT_eSPI display;
#else
extern DisplayGFX display;
#endif

void initDisplay();
void drawWeatherScreen(float tempC, float tempMin, float tempMax, bool isRain, float progress);
void drawLoadingAnimation();
void displayMessage(String message);
void drawChatGptScreen();
void drawBitmapImage(const uint8_t* bitmap, int width, int height);
