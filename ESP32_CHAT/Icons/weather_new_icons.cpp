#include "weather_new_icons.h"
#include <Arduino.h>

const uint16_t iconSunRGB565[] PROGMEM = {
#include "sun565_u16.h"
};

const uint16_t iconRainRGB565[] PROGMEM = {
#include "cloudrain565_u16.h"
};
