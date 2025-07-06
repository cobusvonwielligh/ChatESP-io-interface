#include "display.h"
#include "chatgpt.h"
#include "secrets.h"
#include "weather.h"
#include "app.h" // for DEBUG_MODE flag
#include "Fonts/FreeSansBold.h"

#define SCALE(f) ((int)((f) * 1.8f))

/* ================================================================
 *                  Display Utilities: ILI9488
 * ---------------------------------------------------------------
 * Provides helper functions to initialize the display and render
 * different UI screens and graphics
 * ================================================================ */

TFT_eSPI display = TFT_eSPI();
static TFT_eSprite canvas = TFT_eSprite(&display);
static bool spriteReady = false;

void initDisplay() {
  display.begin();
  display.setRotation(3);
  display.setFreeFont(&FreeSansBold);
#if !DEBUG_MODE
  Serial.println("initDisplay: creating sprite");
  canvas.setColorDepth(32);
  canvas.setFreeFont(&FreeSansBold);
  if (!canvas.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT)) {
    Serial.println("Sprite allocation failed - reduce resolution or enable PSRAM");
    spriteReady = false;
  } else {
    spriteReady = true;
  }
#endif
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

static void fillGradient(TFT_eSPI& d, uint16_t c1, uint16_t c2) {
  uint8_t sr = ((c1 >> 11) & 0x1F) << 3;
  uint8_t sg = ((c1 >> 5) & 0x3F) << 2;
  uint8_t sb = (c1 & 0x1F) << 3;
  uint8_t er = ((c2 >> 11) & 0x1F) << 3;
  uint8_t eg = ((c2 >> 5) & 0x3F) << 2;
  uint8_t eb = (c2 & 0x1F) << 3;

  for (int y = 0; y < SCREEN_HEIGHT; ++y) {
    float t = (float)y / SCREEN_HEIGHT;
    float wave = 0.05f * sinf((millis() / 8000.0f) + t * 6.283f);
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
static void drawSunIcon(TFT_eSPI& d, int x, int y) {
  uint16_t yellow = rgb565(255, 210, 0);
  uint16_t orange = rgb565(255, 170, 0);
  int cx = x + SCALE(30);
  int cy = y + SCALE(30);
  d.fillCircle(cx, cy, SCALE(20), yellow);
  for (int i = 0; i < 12; ++i) {
    float a = sunAngle + i * (6.283185f / 12.0f);
    int x1 = cx + cosf(a) * SCALE(24);
    int y1 = cy + sinf(a) * SCALE(24);
    int x2 = cx + cosf(a) * SCALE(34);
    int y2 = cy + sinf(a) * SCALE(34);
    d.drawLine(x1, y1, x2, y2, orange);
  }
  sunAngle += 0.01f;
}

static void drawRainIcon(TFT_eSPI& d, int x, int y) {
  uint16_t grey = rgb565(200, 200, 200);
  int cx = x + SCALE(30);
  int cy = y + SCALE(25);
  d.fillCircle(cx - SCALE(12), cy, SCALE(18), grey);
  d.fillCircle(cx + SCALE(12), cy, SCALE(18), grey);
  d.fillCircle(cx, cy - SCALE(12), SCALE(18), grey);
  d.fillRect(cx - SCALE(24), cy, SCALE(48), SCALE(20), grey);

  uint16_t blue = rgb565(80, 150, 255);
  int off = (millis() / 250) % 10;
  for (int i = 0; i < 4; ++i) {
    int yy = y + SCALE(45) + ((off + i * 3) % 10);
    d.drawFastVLine(x + SCALE(14) + i * SCALE(12), yy, SCALE(14), blue);
  }
}

static const uint16_t COLOR_BG = rgb565(52, 53, 65);
static const uint16_t COLOR_ACCENT = rgb565(16, 163, 127);

static void drawAssistantButton(TFT_eSPI& d) {
  int s = 50; int y = SCREEN_HEIGHT - s - 8; int x = SCREEN_WIDTH - s - 8;
  d.fillRoundRect(x, y, s, s, 8, COLOR_BG);
  d.drawCircle(x + s / 2, y + s / 2 - 4, 12, COLOR_ACCENT);
  d.fillRect(x + s / 2 - 8, y + s / 2 + 8, 16, 4, COLOR_ACCENT);
}

static void drawHomeButton(TFT_eSPI& d) {
  int s = 50; int y = SCREEN_HEIGHT - s - 8; int x = 8;
  d.fillRoundRect(x, y, s, s, 8, COLOR_BG);
  int cx = x + s / 2;
  d.fillTriangle(cx, y + 12, x + 12, y + 30, x + s - 12, y + 30, COLOR_ACCENT);
  d.fillRect(x + 16, y + 30, s - 32, 14, COLOR_ACCENT);
}

void drawWeatherScreen(float tempC, float tempMin, float tempMax, bool isRain, float progress) {
  if (!spriteReady) {
    Serial.println("drawWeatherScreen skipped - sprite not ready");
    return;
  }
  canvas.fillScreen(TFT_BLACK);

  uint16_t bg1 = rgb565(50, 50, 90);
  uint16_t bg2 = rgb565(5, 5, 60);
  fillGradient(canvas, bg1, bg2);
  canvas.setTextColor(TFT_WHITE);

  const int margin = 18;
  int top = margin + 18;
  
  canvas.setTextSize(1);
  uint16_t w = canvas.textWidth(LOCATION_NAME.c_str());
  canvas.setCursor(SCREEN_WIDTH - w - margin, top);
  canvas.println(LOCATION_NAME);

  canvas.setTextSize(3);
  canvas.setCursor(margin - 10, top + 40);
  canvas.printf("%.1f", tempC);
  canvas.setTextSize(2);
  canvas.print((char)0xB0);
  canvas.print("°");
  canvas.print("C");

  int y = 115;
  canvas.setTextSize(1);
  canvas.setCursor(margin, y);
  canvas.printf("Min %.0f°C", tempMin);
  canvas.setCursor(margin, y + 25);
  canvas.printf("Max %.0f°C", tempMax);

  int iconX = SCREEN_WIDTH - 95 - margin;
  int iconY = top + 40;
  if (isRain) {
    drawRainIcon(canvas, iconX, iconY);
  } else {
    drawSunIcon(canvas, iconX, iconY);
  }

  const int x0 = 0, hBar = 5;
  canvas.fillRect(x0, SCREEN_HEIGHT - hBar, SCREEN_WIDTH, hBar, rgb565(10, 10, 90));
  canvas.fillRect(x0, SCREEN_HEIGHT - hBar, SCREEN_WIDTH * progress, hBar, COLOR_ACCENT);

  drawAssistantButton(canvas);

  display.startWrite();
  canvas.pushSprite(0, 0);
  display.endWrite();
}

void drawLoadingAnimation() {
  if (!spriteReady) {
    Serial.println("drawLoadingAnimation skipped - sprite not ready");
    return;
  }
  canvas.fillScreen(TFT_BLACK);
  canvas.setTextSize(1);
  canvas.setCursor(10, 20);
  canvas.print("ChatGPT thinking");
  int x = 10;
  for (int i = 0; i < 3; i++) {
    canvas.setCursor(x += 30, 20);
    canvas.print(".");
    delay(500);
  }
  display.startWrite();
  canvas.pushSprite(0, 0);
  display.endWrite();
}

void displayMessage(String message) {
#if DEBUG_MODE
  display.startWrite();
  display.fillScreen(TFT_BLACK);
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(TFT_WHITE, TFT_BLACK);
  display.println(message);
  display.endWrite();
#else
  if (!spriteReady) {
    Serial.println("displayMessage skipped - sprite not ready");
    return;
  }
  canvas.fillScreen(TFT_BLACK);
  canvas.setCursor(0, 0);
  canvas.setTextSize(1);
  canvas.println(message);
  display.startWrite();
  canvas.pushSprite(0, 0);
  display.endWrite();
#endif
}

void drawChatGptScreen() {
  if (!spriteReady) {
    Serial.println("drawChatGptScreen skipped - sprite not ready");
    return;
  }
  if (millis() - getLastTypingTime() > getTypingDelay()) {
    updateLastTypingTime();

    canvas.fillScreen(TFT_BLACK);
    canvas.setTextSize(1);
    canvas.setCursor(0, 0);
    canvas.println("ChatGPT:");
    canvas.setCursor(0, 15);
    canvas.println(getChatGptPartialResponse());
    drawHomeButton(canvas);
    display.startWrite();
    canvas.pushSprite(0, 0);
    display.endWrite();
  }
}

void drawBitmapImage(const uint8_t* bitmap, int width, int height) {
  if (!spriteReady) {
    Serial.println("drawBitmapImage skipped - sprite not ready");
    return;
  }
  canvas.fillScreen(TFT_BLACK);
  int x = (SCREEN_WIDTH - width) / 2;
  int y = (SCREEN_HEIGHT - height) / 2;
  canvas.drawXBitmap(x, y, bitmap, width, height, TFT_WHITE, TFT_BLACK);
  drawHomeButton(canvas);
  display.startWrite();
  canvas.pushSprite(0, 0);
  display.endWrite();
}
