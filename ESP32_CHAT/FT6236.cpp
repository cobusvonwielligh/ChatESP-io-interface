/**
 * @file FT6236.cpp
 * @brief FT6236 Touch Controller Implementation
 * 
 * This file implements the functions for reading touch coordinates from the FT6236
 * capacitive touch controller using I2C communication.
 * 
 * @author Lucienne Swart
 * @date March 18, 2025
 * @version 1.0
 * @license MIT
 */

#include "FT6236.h"

/**
 * @brief Reads a register from the FT6236 touch controller
 * 
 * @param reg Register address to read
 * @return int Value read from the register
 */
int readTouchReg(int reg)
{
    int data = 0;
    Wire.beginTransmission(TOUCH_I2C_ADD);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(TOUCH_I2C_ADD, 1);
    if (Wire.available())
    {
        data = Wire.read();
    }
    return data;
}

/*
int getTouchPointX()
{
    int XL = 0;
    int XH = 0;

    XH = readTouchReg(TOUCH_REG_XH);
    XL = readTouchReg(TOUCH_REG_XL);

    return ((XH & 0x0F) << 8) | XL;
}
*/

/**
 * @brief Gets the X coordinate of the touch point
 * 
 * Reads the X coordinate registers and combines them into a single value.
 * Returns -1 if no touch is detected.
 * 
 * @return int X coordinate, or -1 if no touch detected
 */
int getTouchPointX()
{
    int XL = 0;
    int XH = 0;

    XH = readTouchReg(TOUCH_REG_XH);
    //Serial.println(XH >> 6,HEX);
    if (XH >> 6 == 1)
        return -1;
    XL = readTouchReg(TOUCH_REG_XL);

    return ((XH & 0x0F) << 8) | XL;
}

/**
 * @brief Gets the Y coordinate of the touch point
 * 
 * Reads the Y coordinate registers and combines them into a single value.
 * 
 * @return int Y coordinate
 */
int getTouchPointY()
{
    int YL = 0;
    int YH = 0;

    YH = readTouchReg(TOUCH_REG_YH);
    YL = readTouchReg(TOUCH_REG_YL);

    return ((YH & 0x0F) << 8) | YL;
}

/**
 * @brief Gets both X and Y coordinates of the touch point
 * 
 * Reads all coordinate registers and combines them into x,y coordinates.
 * Sets coordinates to -1 if no touch is detected.
 * 
 * @param pos Array to store coordinates [x, y]
 */
void ft6236_pos(int pos[2])
{
    int XL = 0;
    int XH = 0;
    int YL = 0;
    int YH = 0;

    XH = readTouchReg(TOUCH_REG_XH);
    if (XH >> 6 == 1)
    {
        pos[0] = -1;
        pos[1] = -1;
        return;
    }
    XL = readTouchReg(TOUCH_REG_XL);
    YH = readTouchReg(TOUCH_REG_YH);
    YL = readTouchReg(TOUCH_REG_YL);

    pos[0] = ((XH & 0x0F) << 8) | XL;
    pos[1] = ((YH & 0x0F) << 8) | YL;
}
