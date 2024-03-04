#ifndef I2C_HEADER_H
#define I2C_HEADER_H

#include <stdint.h>
#include "comm_interface.h"

#define LIS3MDL_ADDRESS 0x1E // I2C address of the LIS3MDLTR sensor


comm_status_t i2c_read(
    uint8_t bus_address,
    uint8_t register_address,
    uint16_t length,
    uint8_t *buffer);

comm_status_t i2c_write(
    uint8_t bus_address,
    uint8_t register_address,
    uint16_t length,
    uint8_t *buffer);
    
comm_status_t i2c_init(void);
#endif
