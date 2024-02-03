#include "driver.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "i2c.h"

volatile uint8_t bus_address = 0;

// Register addresses
const uint8_t CTRL_REG1 = 0x20;
const uint8_t CTRL_REG2 = 0x21;
const uint8_t INT_CFG = 0x30;
const uint8_t OUT_X_L = 0x28;
const uint8_t OUT_X_H = 0x29;
const uint8_t OUT_Y_L = 0x2A;
const uint8_t OUT_Y_H = 0x2B;
const uint8_t OUT_Z_L = 0x2C;
const uint8_t OUT_Z_H = 0x2D;
// Masks used for generating field structs
const uint8_t DATA_RATE_MASK = 0b01111110;
const uint8_t FS_MASK = 0b01100000;
const uint8_t IEN_MASK = 0b00000001;
// Field defenitions
field IEN = {.register_addr = INT_CFG, .byte_mask = IEN_MASK};
field DATA_RATE = {.register_addr = CTRL_REG1, .byte_mask = DATA_RATE_MASK};
field FS = {.register_addr = CTRL_REG2, .byte_mask = FS_MASK};

uint8_t count_trailing_zeros(uint8_t x) {
#if __has_builtin(__builtin_ctz)
    return __builtin_ctz(x);
#else
    return log2(x & -x);
#endif
}

void set_bus_address(const uint8_t address) {
    bus_address = address;
}

status_t set_register(const field field, const uint8_t value) {
    uint8_t register_value;
    // Read back current value at the given register
    if (i2c_read_byte(bus_address, field.register_addr, &register_value) == STATUS_ERROR) return STATUS_ERROR;
    // Clear existing field value
    register_value &= ~field.byte_mask;
    // Set new configuration (shift by one due to ST bit)
    register_value |= value << count_trailing_zeros(field.byte_mask);
    return i2c_write_byte(bus_address, field.register_addr, register_value);
}

status_t get_register(const field field, uint8_t* value) {
    uint8_t register_value;
    if (i2c_read_byte(bus_address, field.register_addr, &register_value) == STATUS_ERROR) return STATUS_ERROR;
    // Get releavant bits from the register and shift by number of leading zeros in the mask
    *value = (register_value & field.byte_mask) >> count_trailing_zeros(field.byte_mask);
    return STATUS_OK;
}

status_t get_full_scale_conf(full_scale_conf* config) {
    return get_register(FS, (uint8_t*)config);
}

status_t get_data_rate(data_rate_conf* config) {
    if (get_register(DATA_RATE, (uint8_t*)config) == STATUS_ERROR) return STATUS_ERROR;
    if (*config > _80) {
        // For configs that are greater than 80Hz, we don't care about bits at DO
        // But the enum expects them to be 0, so we force them to be 0 by using _1K as a mask
        // This is only required since get_register always returns 0xff instead of actually
        // reading the value stored at the register. This won't be required on the actual IC
        *config &= _1K;
    }
    return STATUS_OK;
}

status_t set_data_rate(const data_rate_conf config) {
    return set_register(DATA_RATE, config);
}

status_t set_interrupt(const bool enabled) {
    return set_register(IEN, enabled);
}

status_t get_axis_data(const axis ax, int16_t* value) {
    uint8_t register_addr;
    switch (ax) {
        case X:
            register_addr = OUT_X_L;
            break;
        case Y:
            register_addr = OUT_Y_L;
            break;
        case Z:
            register_addr = OUT_Z_L;
            break;
    }
    uint8_t buffer[2];
    if (i2c_read_bytes(bus_address, register_addr, 2, buffer) == STATUS_ERROR) return STATUS_ERROR;
    // First data previously stored at value is cleared
    *value = 0;
    // Then data is reordered and put into value (this is because low byte is read fist)
    *value |= buffer[1] << 8;
    *value |= buffer[0];
    return STATUS_OK;
}