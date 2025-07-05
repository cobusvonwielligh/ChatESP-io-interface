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
#if !BASIC_TEST
  lvgl_ui::begin();
  initAudio();

  connectWiFi();
  if (!fetchLocation()) {
    Serial.println("Location lookup failed. Using defaults.");
  }
  initChatGpt();
#else
  displayMessage("Basic display test");
#endif

  state.page = Page::Weather;
  state.lastWeather = 0;
  state.weatherFail = 0;
}

void loop() {
#if !BASIC_TEST
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
#else
  // Basic test simply keeps the message on screen
  delay(1000);
#endif
}

static void connectWiFi() {
#if BASIC_TEST
  // Skip WiFi in basic test
  displayMessage("WiFi disabled (BASIC_TEST)");
#else
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
#endif
}

static void processSerial() {
#if BASIC_TEST
  // Serial commands disabled in basic mode
  return;
#else
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
#endif
}

static void processTouch() {
#if BASIC_TEST
  // Skip touch handling in basic test
  return;
#else
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
#endif
}

} // namespace app
