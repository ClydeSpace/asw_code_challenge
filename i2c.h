#ifndef I2C_HEADER_H
#define I2C_HEADER_H

#include <stdint.h>
#include "data_structures.h"

#define LIS3MDL_REG_WHO_AM_I 0x0F
#define DEFAULT_CTRL_REG1_VALUE 0x00
#define CTRL_REG2   0x21                                
#define CTRL_REG1   0x20                                      
#define INT_CFG     0x30

#define OUT_X_L     0x28
#define OUT_X_H     0x29
#define OUT_Y_L     0x2A
#define OUT_Y_H     0x2B
#define OUT_Z_L     0x2C
#define OUT_Z_H     0x2D

typedef enum {
    STATUS_OK,
    STATUS_ERROR
} status_t;

status_t i2c_read(
    uint8_t bus_address,
    uint8_t register_address,
    uint16_t length,
    uint8_t *buffer);

status_t i2c_write(
    uint8_t bus_address,
    uint8_t register_address,
    uint16_t length,
    uint8_t *buffer);

status_t lis3mdl_init(uint8_t bus_address
                    );

status_t lis3mdl_get_full_scale(uint8_t bus_address, 
                                lis3mdl_full_scale_t *full_scale);

status_t lis3mdl_get_output_data_rate(uint8_t bus_address, 
                                      uint8_t *output_data_rate);

status_t lis3mdl_set_output_data_rate(uint8_t bus_address,
                                      uint8_t output_data_rate);

status_t lis3mdl_configure_interrupt(uint8_t bus_address, 
                                     lis3mdl_interrupt_config_t *interrupt_config);

status_t lis3mdl_read_axis_data(uint8_t bus_address, 
                                lis3mdl_axis_data_t *axis_data);

#endif
