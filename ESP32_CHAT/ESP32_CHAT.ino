#include <WiFi.h>
#include <Wire.h>


#include "display.h"
#include "chatgpt.h"
#include "weather.h"
#include "secrets.h"

/*
  Main Sketch
*/

Page currentPage = PAGE_WEATHER;

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);

  display.init();

  initDisplay(display);
  connectToWiFi();
  initChatGpt();
}

void loop() {
  static unsigned long lastUpdate = 0;
  static float tempC = 0, tempMin = 0, tempMax = 0;
  static uint8_t code = 0;
  static bool isRain = false;
  static int failCount = 0;

  handleSerialInput();

  if (currentPage == PAGE_CHATGPT) {
    if (isTyping()) drawChatGptScreen();
  } else {
    handleWeatherUpdate(tempC, tempMin, tempMax, code, isRain, lastUpdate, failCount);
  }

  delay(100);
}

void connectToWiFi() {
  displayMessage("Connecting to WiFi...");
  WiFi.setSleep(false);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  for (int retries = 0; WiFi.status() != WL_CONNECTED && retries < 16; retries++) {
    delay(1000);
    Serial.print(".");
  }

  if (WiFi.status() != WL_CONNECTED) {
    displayMessage("WiFi failed.\n SSID: " + String(WIFI_SSID));
    while (true) delay(1000);
  }

  displayMessage("WiFi connected.\n Getting weather...");
}

void handleSerialInput() {
  if (Serial.available() > 0) {
    String prompt = Serial.readStringUntil('\n');
    prompt.trim();
    Serial.print("USER: ");
    Serial.println(prompt);

    currentPage = PAGE_CHATGPT;
    resetChatState();
    drawLoadingAnimation();
    if (prompt.startsWith("IMAGE:")) {
      String desc = prompt.substring(6);
      desc.trim();
      const int W = 32, H = 32;
      static uint8_t imgBuf[W * H / 8];
      if (callChatGptImage(desc, imgBuf, W, H)) {
        drawBitmapImage(imgBuf, W, H);
      } else {
        displayMessage("Image error");
      }
    } else {
      callChatGpt(prompt);
    }
  }
}
