/**
 * @file FT6236.h
 * @brief FT6236 Touch Controller Interface
 * 
 * This header file provides the interface for the FT6236 capacitive touch controller.
 * It includes register definitions and function declarations for reading touch coordinates.
 */

#include <Wire.h>

/** I2C address of the FT6236 touch controller */
#define TOUCH_I2C_ADD 0x38

/** Register address for X coordinate low byte */
#define TOUCH_REG_XL 0x04
/** Register address for X coordinate high byte */
#define TOUCH_REG_XH 0x03
/** Register address for Y coordinate low byte */
#define TOUCH_REG_YL 0x06
/** Register address for Y coordinate high byte */
#define TOUCH_REG_YH 0x05

/**
 * @brief Reads a register from the FT6236 touch controller
 * 
 * @param reg Register address to read
 * @return int Value read from the register
 */
int readTouchReg(int reg);

/**
 * @brief Gets the X coordinate of the touch point
 * 
 * @return int X coordinate, or -1 if no touch detected
 */
int getTouchPointX();

/**
 * @brief Gets the Y coordinate of the touch point
 * 
 * @return int Y coordinate
 */
int getTouchPointY();

/**
 * @brief Gets both X and Y coordinates of the touch point
 * 
 * @param pos Array to store coordinates [x, y]
 */
void ft6236_pos(int pos[2]);
