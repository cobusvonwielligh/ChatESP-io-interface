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
  d.setFont(&fonts::FreeSansBold12pt7b);
}

static uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

static uint16_t bgColorForTemp(float t) {
  if (t < 5)  return rgb565(0, 0, 80);
  if (t < 15) return rgb565(0, 64, 160);
  if (t < 25) return rgb565(0, 160, 80);
  if (t < 35) return rgb565(255, 140, 0);
  return rgb565(200, 0, 0);
}

static void fillGradient(LGFX& d, uint16_t c1, uint16_t c2) {
  uint8_t sr = ((c1 >> 11) & 0x1F) << 3;
  uint8_t sg = ((c1 >> 5) & 0x3F) << 2;
  uint8_t sb = (c1 & 0x1F) << 3;
  uint8_t er = ((c2 >> 11) & 0x1F) << 3;
  uint8_t eg = ((c2 >> 5) & 0x3F) << 2;
  uint8_t eb = (c2 & 0x1F) << 3;

  for (int y = 0; y < SCREEN_HEIGHT; ++y) {
    float t = (float)y / SCREEN_HEIGHT;
    float wave = 0.1f * sinf((millis() / 3000.0f) + t * 6.283f);
    t += wave;
    if (t < 0) t = 0; if (t > 1) t = 1;
    uint8_t r = sr + (er - sr) * t;
    uint8_t g = sg + (eg - sg) * t;
    uint8_t b = sb + (eb - sb) * t;
    uint16_t col = rgb565(r, g, b);
    d.drawFastHLine(0, y, SCREEN_WIDTH, col);
  }
}

static float sunAngle = 0.0f;
static void drawSunIcon(LGFX& d, int x, int y) {
  uint16_t yellow = rgb565(255, 200, 0);
  uint16_t orange = rgb565(255, 150, 0);
  int cx = x + 25;
  int cy = y + 25;
  d.fillCircle(cx, cy, 18, yellow);
  for (int i = 0; i < 8; ++i) {
    float a = sunAngle + i * 0.785398f; // PI/4
    int x1 = cx + cosf(a) * 22;
    int y1 = cy + sinf(a) * 22;
    int x2 = cx + cosf(a) * 32;
    int y2 = cy + sinf(a) * 32;
    d.drawLine(x1, y1, x2, y2, orange);
  }
  sunAngle += 0.02f;
}

static void drawRainIcon(LGFX& d, int x, int y) {
  uint16_t grey = rgb565(180, 180, 180);
  int cx = x + 25;
  int cy = y + 20;
  d.fillCircle(cx - 10, cy, 15, grey);
  d.fillCircle(cx + 10, cy, 15, grey);
  d.fillCircle(cx, cy - 10, 15, grey);
  d.fillRect(cx - 20, cy, 40, 15, grey);

  uint16_t blue = rgb565(80, 150, 255);
  int off = (millis() / 200) % 8;
  for (int i = 0; i < 3; ++i) {
    int yy = y + 35 + ((off + i * 3) % 8);
    d.drawFastVLine(x + 12 + i * 12, yy, 12, blue);
  }
}

void drawWeatherScreen(float tempC, float tempMin, float tempMax, bool isRain, float progress) {
  LGFX& disp = *displayRef;

  disp.startWrite();

  uint16_t bg1 = bgColorForTemp(tempC);
  uint16_t bg2 = rgb565(0, 0, 0);
  fillGradient(disp, bg1, bg2);
  disp.setTextColor(TFT_WHITE);

  const int margin = 8;
  disp.setTextSize(1);
  disp.setCursor(margin, margin);
  disp.println("Weather");

  uint16_t w = disp.textWidth(PLACE_NAME);
  disp.setCursor(SCREEN_WIDTH - w - margin, margin);
  disp.println(PLACE_NAME);

  disp.setTextSize(4);
  disp.setCursor(margin, 40);
  disp.printf("%.1f", tempC);
  disp.setTextSize(3);
  disp.print((char)176);
  disp.print("C");

  int y = 120;
  disp.setTextSize(2);
  disp.setCursor(margin, y);
  disp.printf("Min: %.0f%cC", tempMin, (char)176);
  disp.setCursor(margin, y + 30);
  disp.printf("Max: %.0f%cC", tempMax, (char)176);

  int iconX = SCREEN_WIDTH - 60 - margin;
  int iconY = margin + 20;
  if (isRain) {
    drawRainIcon(disp, iconX, iconY);
  } else {
    drawSunIcon(disp, iconX, iconY);
  }

  const int x0 = 0, hBar = 4;
  disp.fillRect(x0, SCREEN_HEIGHT - hBar, SCREEN_WIDTH, hBar, rgb565(50, 50, 50));
  disp.fillRect(x0, SCREEN_HEIGHT - hBar, SCREEN_WIDTH * progress, hBar, rgb565(255, 255, 255));

  disp.endWrite();
}

void drawLoadingAnimation() {
  LGFX& disp = *displayRef;
  disp.startWrite();
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
  disp.endWrite();
}

void displayMessage(String message) {
  LGFX& disp = *displayRef;
  disp.startWrite();
  disp.fillScreen(TFT_BLACK);
  disp.setCursor(0, 0);
  disp.setTextSize(1);
  disp.println(message);
  disp.endWrite();
}

void drawChatGptScreen() {
  LGFX& disp = *displayRef;
  if (millis() - getLastTypingTime() > getTypingDelay()) {
    updateLastTypingTime();

    disp.startWrite();
    disp.fillScreen(TFT_BLACK);
    disp.setTextSize(1);
    disp.setCursor(0, 0);
    disp.println("ChatGPT:");
    disp.setCursor(0, 15);
    disp.println(getChatGptPartialResponse());
    disp.endWrite();
  }
}

void drawBitmapImage(const uint8_t* bitmap, int width, int height) {
  LGFX& disp = *displayRef;
  disp.startWrite();
  disp.fillScreen(TFT_BLACK);
  int x = (SCREEN_WIDTH - width) / 2;
  int y = (SCREEN_HEIGHT - height) / 2;
  disp.drawXBitmap(x, y, bitmap, width, height, TFT_WHITE, TFT_BLACK);
  disp.endWrite();
}

