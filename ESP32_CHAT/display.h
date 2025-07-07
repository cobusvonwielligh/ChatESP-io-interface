#pragma once
#include "makerfabs_pin.h"
#include <TFT_eSPI.h>

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

extern TFT_eSPI display;

void initDisplay();
