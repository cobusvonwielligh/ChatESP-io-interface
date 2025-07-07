#include "display.h"

TFT_eSPI display = TFT_eSPI();

void initDisplay() {
    display.begin();
    display.setRotation(3);
}
