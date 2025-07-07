#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>

#include "weather.h"
#include "GuiService.h"
#include "secrets.h"

/* ============================================
 *          Lightweight Weather Client
 * ============================================ */


String LOCATION_NAME = DEFAULT_PLACE_NAME;
String LOCATION_LAT  = DEFAULT_PLACE_LAT;
String LOCATION_LNG  = DEFAULT_PLACE_LNG;

bool fetchLocation() {
  if (WiFi.status() != WL_CONNECTED) return false;

  HTTPClient http;
  http.begin("http://ip-api.com/json/");
  int status = http.GET();
  if (status != 200) {
    http.end();
    return false;
  }
  DynamicJsonDocument doc(1024);
  if (deserializeJson(doc, http.getStream())) {
    http.end();
    return false;
  }
  LOCATION_NAME = doc["city"].as<String>();
  LOCATION_LAT = String(doc["lat"].as<float>(), 6);
  LOCATION_LNG = String(doc["lon"].as<float>(), 6);
  http.end();
  return true;
}

bool fetchWeather(float &tempC, uint8_t &code, float &tempMin, float &tempMax) {
  if (WiFi.status() != WL_CONNECTED) return false;

  String url = String("http://api.open-meteo.com/v1/forecast?latitude=") +
               LOCATION_LAT + "&longitude=" + LOCATION_LNG +
               "&current_weather=true&daily=temperature_2m_min,temperature_2m_max&timezone=auto";

  HTTPClient http;
  http.begin(url);
  http.useHTTP10();

  int status = http.GET();
  if (status != 200) {
    http.end();
    return false;
  }

  DynamicJsonDocument doc(2048);
  if (deserializeJson(doc, http.getStream())) {
    http.end();
    return false;
  }

  tempC = doc["current_weather"]["temperature"] | NAN;
  code = doc["current_weather"]["weathercode"] | 0;
  tempMin = doc["daily"]["temperature_2m_min"][0] | NAN;
  tempMax = doc["daily"]["temperature_2m_max"][0] | NAN;

  http.end();
  return !isnan(tempC);
}

void handleWeatherUpdate(float &tempC, float &tempMin, float &tempMax, uint8_t &code, bool &isRain,
                         unsigned long &lastUpdate, int &failCount) {
  unsigned long now = millis();
  float progress = float(now - lastUpdate) / WEATHER_PAGE_REFRESH_MS;
  if (progress > 1.0) progress = 1.0;

  if (now - lastUpdate > WEATHER_PAGE_REFRESH_MS || lastUpdate == 0) {
    if (fetchWeather(tempC, code, tempMin, tempMax)) {
      isRain = (code >= 51 && code <= 67) || (code >= 80 && code <= 82);
      lastUpdate = now;
      failCount = 0;
    } else {
      failCount++;
      if (failCount > 3) {
        Serial.println("Failed to fetch weather.");
        delay(10000);
      }
    }
  }

  UI::updateWeather(tempC, tempMin, tempMax, isRain, progress, LOCATION_NAME);
}
