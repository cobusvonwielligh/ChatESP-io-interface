#pragma once
#include "makerfabs_pin.h"

#define FT6236_TOUCH
//#define NS2009_TOUCH

#ifdef FT6236_TOUCH
#include "FT6236.h"
inline void readTouch(int pos[2]) { ft6236_pos(pos); }
#endif
#ifdef NS2009_TOUCH
#include "NS2009.h"
inline void readTouch(int pos[2]) { ns2009_pos(pos); }
#endif
