#ifndef LIS3MDL_DRIVER
#define LIS3MDL_DRIVER

#include <stdbool.h>
#include <stdint.h>

#include "i2c.h"

// Each register only contains 8 bits of data so all enum values
// must be 255 or less, otherwise they will be truncated

typedef struct {
    const uint8_t register_addr;
    const uint8_t byte_mask;
} field;

// Values of data_rate_conf are used as bitmasks and are generated from datasheet table 19, 20 and 21
typedef enum {
    _0P625 = 0b000000,
    _1P25 = 0b000010,
    _2P5 = 0b000100,
    _5 = 0b000110,
    _10 = 0b001000,
    _20 = 0b001010,
    _40 = 0b001100,
    _80 = 0b001110,
    // Setting data rates above 80Hz will impact X and Y axes
    // operating mode (refer to datasheet table 19, 20 and 21)
    _155 = 0b110001,
    _300 = 0b100001,
    _560 = 0b010001,
    _1K = 0b110001
} data_rate_conf;  // Refers to CTRL_REG1 masked by DATA_RATE_MASK

// Values of full_scale_conf are used as bitmasks and are generated from datasheet table 24
typedef enum {
    _4Gs = 0b00,
    _8Gs = 0b01,
    _12Gs = 0b10,
    _16Gs = 0b11
} full_scale_conf;

typedef enum {
    X,
    Y,
    Z
} axis;

void set_bus_address(const uint8_t address);

status_t get_full_scale_conf(full_scale_conf* config);

status_t get_data_rate(data_rate_conf* config);

status_t set_data_rate(const data_rate_conf config);

status_t set_interrupt(const bool enabled);

status_t get_axis_data(const axis ax, int16_t* value);

#endif