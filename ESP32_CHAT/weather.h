#pragma once
#include <Arduino.h>

/* ============================================
 *           Weather API Interface
 * ============================================ */

/* Fetch weather from Open-Meteo API */
bool fetchWeather(float &tempC, uint8_t &code, float &tempMin, float &tempMax);

/* Obtain approximate location using IP geolocation */
bool fetchLocation();

extern String LOCATION_NAME;
extern String LOCATION_LAT;
extern String LOCATION_LNG;

/* Handle update cycle and screen refresh */
void handleWeatherUpdate(float &tempC, float &tempMin, float &tempMax, uint8_t &code, bool &isRain,
                         unsigned long &lastUpdate, int &failCount);
