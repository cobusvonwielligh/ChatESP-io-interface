#pragma once
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET false
#define OLED_ADDR 0x3C

/* OLED drawing helpers */

extern Adafruit_SSD1306 display;

void initDisplay(Adafruit_SSD1306& d);
void drawWeatherScreen(float tempC, float tempMin, float tempMax, bool isRain, float progress);
void drawLoadingAnimation();
void displayMessage(String message);
void drawChatGptScreen();
void drawBitmapImage(const uint8_t* bitmap, int width, int height);
