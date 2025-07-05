/**
 * @file NS2009.cpp
 * @brief NS2009 Touch Controller Implementation
 * 
 * This file implements the functions for reading touch coordinates and pressure
 * from the NS2009 resistive touch controller using I2C communication.
 * 
 * @author Lucienne Swart
 * @date March 18, 2025
 * @version 1.0
 * @license MIT
 */

#include "NS2009.h"

/**
 * @brief Receives data from the NS2009 touch controller via I2C
 * 
 * @param send_buf Buffer containing data to send
 * @param send_buf_len Length of send buffer
 * @param receive_buf Buffer to store received data
 * @param receive_buf_len Length of receive buffer
 */
void ns2009_recv(const uint8_t *send_buf, size_t send_buf_len, uint8_t *receive_buf,
                 size_t receive_buf_len)
{
    Wire.beginTransmission(NS2009_ADDR);
    Wire.write(send_buf, send_buf_len);
    Wire.endTransmission();
    Wire.requestFrom(NS2009_ADDR, receive_buf_len);
    while (Wire.available())
    {
        *receive_buf++ = Wire.read();
    }
}

/**
 * @brief Reads a 12-bit value from the NS2009 touch controller
 * 
 * @param cmd Command to send
 * @return unsigned int 12-bit value read from the controller
 */
unsigned int ns2009_read(uint8_t cmd)
{
    uint8_t buf[2];
    ns2009_recv(&cmd, 1, buf, 2);
    return (buf[0] << 4) | (buf[1] >> 4);
}

/**
 * @brief Gets the current pressure value from the touch controller
 * 
 * @return int Pressure value (may not be accurate)
 */
int ns2009_get_press()
{
    return ns2009_read(NS2009_LOW_POWER_READ_Z1);
}

/**
 * @brief Gets the current touch position and pressure
 * 
 * Reads X and Y coordinates and converts them from raw values (0-4095)
 * to screen coordinates (0-320 for X, 0-480 for Y).
 * 
 * @param pos Array to store coordinates [x, y]
 * @return int Pressure value
 */
int ns2009_pos(int pos[2])
{
    int press = ns2009_read(NS2009_LOW_POWER_READ_Z1);

    int x, y = 0;

    x = ns2009_read(NS2009_LOW_POWER_READ_X);
    y = ns2009_read(NS2009_LOW_POWER_READ_Y);

    pos[0] = x * SCREEN_X_PIXEL / 4096; //4096 = 2 ^ 12
    pos[1] = y * SCREEN_Y_PIXEL / 4096;

    //pos[0] = x;
    //pos[1] = y;
    return press;
}
