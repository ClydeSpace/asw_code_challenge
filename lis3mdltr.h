#ifndef LIS3MDLTR_HEADER_H
#define LIS3MDLTR_HEADER_H

#include <stdint.h>
#include <stdbool.h>
// i2c to be implemented depending on the target platform
#include "i2c.h"

// Define the sensor I2C address
static const uint8_t I2C_ADDR_LOW = 0x1C;
static const uint8_t I2C_ADDR_HIGH = 0x1E;

// Define the internal register addresses
// Output data rate register
static const uint8_t REG_CTRL_REG1 = 0x20;
// Full scale configuration register
static const uint8_t REG_CTRL_REG2 = 0x21;
// Output axis data status register
static const uint8_t REG_STATUS_REG = 0x27;
// Output data X-Axis register
static const uint8_t REG_OUT_X_L = 0x28;
static const uint8_t REG_OUT_X_H = 0x29;
// Output data Y-Axis register
static const uint8_t REG_OUT_Y_L = 0x2A;
static const uint8_t REG_OUT_Y_H = 0x2B;
// Output data Z-Axis register
static const uint8_t REG_OUT_Z_L = 0x2C;
static const uint8_t REG_OUT_Z_H = 0x2D;
// Interrupt configuration register
static const uint8_t REG_INT_CFG = 0x30;

// variables
uint8_t buffer[9];
float sensitivity = 0.0f;

// getter functions
// Assumption: config_bits & rate_bits are 8-bit values
bool get_full_scale_config(uint8_t *scale);
bool get_output_data_rate(uint8_t *op_mode, uint8_t *data_rate, uint8_t *fast_odr);
bool get_data_x_axis(float *value);
bool get_data_y_axis(float *value);
bool get_data_z_axis(float *value);
float get_sensitivity();

// setter functions
bool set_output_data_rate(const uint8_t op_mode, const uint8_t data_rate, const uint8_t fast_odr);
bool set_interrupt(const bool status);

#endif