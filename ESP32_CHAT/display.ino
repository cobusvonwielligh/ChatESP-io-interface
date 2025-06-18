#include "display.h"
#include "weather_icons.h"
#include "chatgpt.h"
#include "secrets.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
static Adafruit_SSD1306* displayRef = nullptr;

void initDisplay(Adafruit_SSD1306& d) {
  displayRef = &d;
}

void drawWeatherScreen(float tempC, float tempMin, float tempMax, bool isRain, float progress) {
  Adafruit_SSD1306& display = *displayRef;
  display.clearDisplay();

  const int margin = 4;
  display.setTextSize(1);
  display.setCursor(margin, margin);
  display.print("Weather");

  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(PLACE_NAME, 0, 0, &x1, &y1, &w, &h);
  display.setCursor(SCREEN_WIDTH - w - margin, margin);
  display.print(PLACE_NAME);

  display.setTextSize(2);
  display.setCursor(margin, 15 + margin);
  display.printf("%.1f", tempC);
  display.setTextSize(1);
  display.print((char)247);
  display.setTextSize(2);
  display.print("C");

  display.setTextSize(1);
  display.setCursor(margin, 38 + margin);
  display.printf("Min: %.0f%cC", tempMin, (char)247);
  display.setCursor(margin, 50 + margin);
  display.printf("Max: %.0f%cC", tempMax, (char)247);

  display.drawBitmap(SCREEN_WIDTH - 50 - margin, 8 + margin,
    isRain ? iconRainBitmap : iconSunBitmap,
    50, 50, SSD1306_WHITE, SSD1306_BLACK);

  const int x0 = 0, hBar = 2;
  display.fillRect(x0, SCREEN_HEIGHT - hBar, SCREEN_WIDTH, hBar, SSD1306_BLACK);
  display.fillRect(x0, SCREEN_HEIGHT - hBar, SCREEN_WIDTH * progress, hBar, SSD1306_WHITE);

  display.display();
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