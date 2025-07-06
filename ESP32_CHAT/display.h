#pragma once
#include "makerfabs_pin.h"
#include <TFT_eSPI.h>

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

extern TFT_eSPI display;

void initDisplay();
void drawWeatherScreen(float tempC, float tempMin, float tempMax, bool isRain, float progress);
void drawLoadingAnimation();
void displayMessage(String message);
void drawChatGptScreen();
void drawBitmapImage(const uint8_t* bitmap, int width, int height);
