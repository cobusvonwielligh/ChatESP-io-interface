#include "display.h"
#include "weather_icons.h"
#include "chatgpt.h"
#include "secrets.h"

/* ================================================================
 *                  Display Utilities: SSD1306
 * ---------------------------------------------------------------
 * Provides helper functions to initialize the display and render
 * different UI screens and graphics
 * ================================================================ */

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
static Adafruit_SSD1306* displayRef = nullptr;

void initDisplay(Adafruit_SSD1306& d) {
  displayRef = &d;
}

void drawWeatherScreen(float tempC, float tempMin, float tempMax, bool isRain, float progress) {
  Adafruit_SSD1306& disp = *displayRef;
  disp.clearDisplay();

  const int margin = 4;
  disp.setTextSize(1);
  disp.setCursor(margin, margin);
  disp.print("Weather");

  int16_t x1, y1;
  uint16_t w, h;
  disp.getTextBounds(PLACE_NAME, 0, 0, &x1, &y1, &w, &h);
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

  disp.drawBitmap(SCREEN_WIDTH - 50 - margin, margin + 8,
    isRain ? iconRainBitmap : iconSunBitmap,
    50, 50, SSD1306_WHITE, SSD1306_BLACK);

  const int x0 = 0, hBar = 2;
  disp.fillRect(x0, SCREEN_HEIGHT - hBar, SCREEN_WIDTH, hBar, SSD1306_BLACK);
  disp.fillRect(x0, SCREEN_HEIGHT - hBar, SCREEN_WIDTH * progress, hBar, SSD1306_WHITE);

  disp.display();
}

void drawLoadingAnimation() {
  Adafruit_SSD1306& display = *displayRef;
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(10, 20);
  display.print("ChatGPT thinking");
  int x = 10;
  for (int i = 0; i < 3; i++) {
    display.setCursor(x += 30, 20);
    display.print(".");
    display.display();
    delay(500);
  }
}

void displayMessage(String message) {
  Adafruit_SSD1306& display = *displayRef;
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println(message);
  display.display();
}

void drawChatGptScreen() {
  Adafruit_SSD1306& display = *displayRef;
  if (millis() - getLastTypingTime() > getTypingDelay()) {
    updateLastTypingTime();

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("ChatGPT:");
    display.setCursor(0, 15);
    display.println(getChatGptPartialResponse());
    display.display();
  }
}

void drawBitmapImage(const uint8_t* bitmap, int width, int height) {
  Adafruit_SSD1306& disp = *displayRef;
  disp.clearDisplay();
  int x = (SCREEN_WIDTH - width) / 2;
  int y = (SCREEN_HEIGHT - height) / 2;
  disp.drawBitmap(x, y, bitmap, width, height, SSD1306_WHITE, SSD1306_BLACK);
  disp.display();
}
