/**
 * @file NS2009.h
 * @brief NS2009 Touch Controller Interface
 * 
 * This header file provides the interface for the NS2009 resistive touch controller.
 * It includes register definitions and function declarations for reading touch coordinates
 * and pressure values.
 * 
 * @author Lucienne Swart
 * @date March 18, 2025
 * @version 1.0
 * @license MIT
 */

#include <Wire.h>

/** I2C address of the NS2009 touch controller */
#define NS2009_ADDR 0x48 //10010000

/** Command to read X coordinate in low power mode */
#define NS2009_LOW_POWER_READ_X 0xc0
/** Command to read Y coordinate in low power mode */
#define NS2009_LOW_POWER_READ_Y 0xd0
/** Command to read Z1 (pressure) in low power mode */
#define NS2009_LOW_POWER_READ_Z1 0xe0

/** Screen width in pixels */
#define SCREEN_X_PIXEL 320
/** Screen height in pixels */
#define SCREEN_Y_PIXEL 480

/**
 * @brief Receives data from the NS2009 touch controller via I2C
 * 
 * @param send_buf Buffer containing data to send
 * @param send_buf_len Length of send buffer
 * @param receive_buf Buffer to store received data
 * @param receive_buf_len Length of receive buffer
 */
void ns2009_recv(const uint8_t *send_buf, size_t send_buf_len, uint8_t *receive_buf,
                size_t receive_buf_len);

/**
 * @brief Reads a value from the NS2009 touch controller
 * 
 * @param cmd Command to send
 * @param val Pointer to store the read value
 * @return unsigned int Status of the read operation
 */
unsigned ns2009_read(uint8_t cmd, int *val);

/**
 * @brief Gets the current touch position
 * 
 * @param pos Array to store coordinates [x, y]
 * @return int Pressure value
 */
int ns2009_pos(int pos[2]);

/**
 * @brief Gets the current pressure value
 * 
 * @return int Pressure value
 */
int ns2009_get_press();
