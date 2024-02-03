#ifndef I2C_HEADER_H
#define I2C_HEADER_H

#include <stdint.h>

typedef enum {
    STATUS_OK,
    STATUS_ERROR
} status_t;

status_t i2c_read_byte(const uint8_t bus_address, const uint8_t register_address, uint8_t *byte);

// When reading N bytes, register_address is used as the base address
// and is incremented by 1 after reading each byte
status_t i2c_read_bytes(
    const uint8_t bus_address,
    const uint8_t register_address,
    const uint16_t length,
    uint8_t *buffer);

status_t i2c_write_byte(const uint8_t bus_address, const uint8_t register_address, const uint8_t byte);

// When writing N bytes, register_address is used as the base address
// and is incremented by 1 after reading each byte
status_t i2c_write_bytes(
    const uint8_t bus_address,
    const uint8_t register_address,
    const uint16_t length,
    const uint8_t *buffer);

#endif
