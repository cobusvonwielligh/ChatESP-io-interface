#include "app.h"
#include "secrets.h"

namespace app {

static State state;

static void connectWiFi();
static void processSerial();
static void processTouch();

void begin() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);

  display.init();
  initDisplay(display);
  lvgl_ui::begin();
  initAudio();

  connectWiFi();
  if (!fetchLocation()) {
    Serial.println("Location lookup failed. Using defaults.");
  }
  initChatGpt();

  state.page = Page::Weather;
  state.lastWeather = 0;
  state.weatherFail = 0;
}

void loop() {
  processTouch();
  processSerial();
  lvgl_ui::loop();

  if (state.page == Page::ChatGpt) {
    if (isTyping()) {
      drawChatGptScreen();
      lvgl_ui::showChat(getChatGptPartialResponse());
    }
  } else {
    handleWeatherUpdate(state.tempC, state.tempMin, state.tempMax,
                        state.weatherCode, state.raining,
                        state.lastWeather, state.weatherFail);
  }
}

static void connectWiFi() {
  displayMessage("Connecting to WiFi...");
  WiFi.setSleep(false);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  for (int retries = 0; WiFi.status() != WL_CONNECTED && retries < 16; ++retries) {
    delay(1000);
    Serial.print(".");
  }

  if (WiFi.status() != WL_CONNECTED) {
    displayMessage(String("WiFi failed.\nSSID: ") + WIFI_SSID);
    while (true) delay(1000);
  }
  displayMessage("WiFi connected.\nGetting weather...");
}

static void processSerial() {
  if (Serial.available() == 0) return;
  String prompt = Serial.readStringUntil('\n');
  prompt.trim();
  Serial.print("USER: ");
  Serial.println(prompt);

  state.page = Page::ChatGpt;
  resetChatState();
  drawLoadingAnimation();
  lvgl_ui::showChat("...");
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
    lvgl_ui::showChat("");
  }
}

static void processTouch() {
  int pos[2] = { -1, -1 };
  readTouch(pos);
  if (pos[0] < 0 || pos[1] < 0) return;

  const int btnSize = 50;
  int y = SCREEN_HEIGHT - btnSize - 8;

  if (state.page == Page::Weather) {
    if (pos[0] > SCREEN_WIDTH - btnSize - 8 && pos[1] > y) {
      state.page = Page::ChatGpt;
      resetChatState();
      drawLoadingAnimation();
      lvgl_ui::showChat("...");
    }
  } else if (state.page == Page::ChatGpt) {
    if (pos[0] < btnSize + 8 && pos[1] > y) {
      state.page = Page::Weather;
      float prog = float(millis() - state.lastWeather) / WEATHER_PAGE_REFRESH_MS;
      if (prog > 1.0f) prog = 1.0f;
      lvgl_ui::updateWeather(state.tempC, state.tempMin, state.tempMax,
                            state.raining, prog);
    }
  }
}

} // namespace app
