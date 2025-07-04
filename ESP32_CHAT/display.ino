#include "display.h"
#include "weather_icons.h"
#include "chatgpt.h"
#include "secrets.h"

/* ================================================================
 *                  Display Utilities: ILI9488
 * ---------------------------------------------------------------
 * Provides helper functions to initialize the display and render
 * different UI screens and graphics
 * ================================================================ */

LGFX display;
static LGFX* displayRef = nullptr;

void initDisplay(LGFX& d) {
  displayRef = &d;
}

void drawWeatherScreen(float tempC, float tempMin, float tempMax, bool isRain, float progress) {
  LGFX& disp = *displayRef;

  disp.fillScreen(TFT_BLACK);

  const int margin = 4;
  disp.setTextSize(1);
  disp.setCursor(margin, margin);
  disp.print("Weather");

  uint16_t w = disp.textWidth(PLACE_NAME);
  disp.setCursor(SCREEN_WIDTH - w - margin, margin);
  disp.print(PLACE_NAME);

  // Current temperature with degree symbol
  disp.setTextSize(2);
  disp.setCursor(margin, margin + 15);
  disp.printf("%.1f", tempC);
  disp.setTextSize(1);
  disp.print((char)176); // degree symbol
  disp.setTextSize(2);
  disp.print("C");

  disp.setTextSize(1);
  disp.setCursor(margin, margin + 38);
  disp.printf("Min: %.0f%cC", tempMin, (char)176);
  disp.setCursor(margin, margin + 50);
  disp.printf("Max: %.0f%cC", tempMax, (char)176);

  disp.drawXBitmap(SCREEN_WIDTH - 50 - margin, margin + 8,
    isRain ? iconRainBitmap : iconSunBitmap,
    50, 50, TFT_WHITE, TFT_BLACK);

  const int x0 = 0, hBar = 2;
  disp.fillRect(x0, SCREEN_HEIGHT - hBar, SCREEN_WIDTH, hBar, TFT_BLACK);
  disp.fillRect(x0, SCREEN_HEIGHT - hBar, SCREEN_WIDTH * progress, hBar, TFT_WHITE);
}

void drawLoadingAnimation() {
  LGFX& disp = *displayRef;

  disp.fillScreen(TFT_BLACK);
  disp.setTextSize(1);
  disp.setCursor(10, 20);
  disp.print("ChatGPT thinking");
  int x = 10;
  for (int i = 0; i < 3; i++) {
    disp.setCursor(x += 30, 20);
    disp.print(".");
    delay(500);
  }
}

void displayMessage(String message) {
  LGFX& disp = *displayRef;

  disp.fillScreen(TFT_BLACK);
  disp.setCursor(0, 0);
  disp.setTextSize(1);
  disp.println(message);
}

void drawChatGptScreen() {
  LGFX& disp = *displayRef;

  if (millis() - getLastTypingTime() > getTypingDelay()) {
    updateLastTypingTime();

    disp.fillScreen(TFT_BLACK);
    disp.setTextSize(1);
    disp.setCursor(0, 0);
    disp.println("ChatGPT:");
    disp.setCursor(0, 15);
    disp.println(getChatGptPartialResponse());
  }
}

void drawBitmapImage(const uint8_t* bitmap, int width, int height) {
  LGFX& disp = *displayRef;

  disp.fillScreen(TFT_BLACK);
  int x = (SCREEN_WIDTH - width) / 2;
  int y = (SCREEN_HEIGHT - height) / 2;
  disp.drawXBitmap(x, y, bitmap, width, height, TFT_WHITE, TFT_BLACK);
}
