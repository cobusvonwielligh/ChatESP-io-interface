#pragma once
#include <Arduino.h>

/* ============================================
 *           Weather API Interface
 * ============================================ */

/* Fetch weather from Open-Meteo API */
bool fetchWeather(float &tempC, uint8_t &code, float &tempMin, float &tempMax);

/* Handle update cycle and screen refresh */
void handleWeatherUpdate(float &tempC, float &tempMin, float &tempMax, uint8_t &code, bool &isRain,
                         unsigned long &lastUpdate, int &failCount);
