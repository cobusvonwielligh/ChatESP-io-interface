#include "app.h"
#include "secrets.h"
#include "GuiService.h"

namespace app {

static State state;

static void connectWiFi();
static void processSerial();
static void processTouch();

void begin() {
  Serial.begin(115200);
  Serial.println("app: begin");
  Wire.begin(I2C_SDA, I2C_SCL);

  Serial.println("app: initDisplay");
  initDisplay();
#if !DEBUG_MODE
  Serial.println("app: UI::begin");
  if (!UI::begin()) {
    Serial.println("LVGL UI disabled due to init failure");
  }
  Serial.println("app: initAudio");
  initAudio();

  Serial.println("app: connectWiFi");
  connectWiFi();
  if (!fetchLocation()) {
    Serial.println("Location lookup failed. Using defaults.");
  }
  Serial.println("app: initChatGpt");
  initChatGpt();
#else
  Serial.println("app: DEBUG_MODE display test");
#endif

  state.page = Page::Weather;
  state.lastWeather = 0;
  state.weatherFail = 0;
  Serial.println("app: ready");
}

void loop() {
#if !DEBUG_MODE
  processTouch();
  processSerial();
  UI::loop();

  if (state.page == Page::ChatGpt) {
    if (isTyping()) {
      UI::showChat(getChatGptPartialResponse());
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
#if DEBUG_MODE
  // Skip WiFi in basic test
  Serial.println("WiFi disabled (DEBUG_MODE)");
#else
  Serial.println("Connecting to WiFi...");
  WiFi.setSleep(false);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  for (int retries = 0; WiFi.status() != WL_CONNECTED && retries < 16; ++retries) {
    delay(1000);
    Serial.print(".");
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(String("WiFi failed. SSID: ") + WIFI_SSID);
    while (true) delay(1000);
  }
  Serial.println("WiFi connected. Getting weather...");
#endif
}

static void processSerial() {
#if DEBUG_MODE
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
  UI::showChat("...");
  callChatGpt(prompt);
#endif
}

static void processTouch() {
#if DEBUG_MODE
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
      UI::showChat("...");
    }
  } else if (state.page == Page::ChatGpt) {
    if (pos[0] < btnSize + 8 && pos[1] > y) {
      state.page = Page::Weather;
      float prog = float(millis() - state.lastWeather) / WEATHER_PAGE_REFRESH_MS;
      if (prog > 1.0f) prog = 1.0f;
      UI::updateWeather(state.tempC, state.tempMin, state.tempMax,
                        state.raining, prog, LOCATION_NAME);
    }
  }
#endif
}

} // namespace app
