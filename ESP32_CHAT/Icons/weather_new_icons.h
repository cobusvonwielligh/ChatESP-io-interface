#ifndef WEATHER_NEW_ICONS_H
#define WEATHER_NEW_ICONS_H

#include <Arduino.h>

// 64×64 RGB565 weather icons stored in PROGMEM.  The icon data lives in
// separate header files to keep this file manageable.  Including the data
// directly here allows the arrays to be compiled even when the Arduino build
// system does not pick up source files from subdirectories.

const uint16_t iconSunRGB565[] PROGMEM = {
#include "sun565_u16.h"
};

const uint16_t iconRainRGB565[] PROGMEM = {
#include "cloudrain565_u16.h"
};

const int iconWidth  = 64;
const int iconHeight = 64;

#endif
