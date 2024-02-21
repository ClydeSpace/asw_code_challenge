#ifndef I2C_HEADER_H
#define I2C_HEADER_H

#include <stdint.h>

#define REG_MAP_SIZE    0x33

/* Register map */
#define OFFSET_X_REG_L_M    0x05
#define OFFSET_X_REG_H_M    0x06
#define OFFSET_Y_REG_L_M    0x07
#define OFFSET_Y_REG_H_M    0x08
#define OFFSET_Z_REG_L_M    0x09
#define OFFSET_Z_REG_H_M    0x0A

#define WHO_AM_I    0x0F

#define CTRL_REG1   0x20
#define CTRL_REG2   0x21
#define CTRL_REG3   0x22
#define CTRL_REG4   0x23
#define CTRL_REG5   0x24

#define STATUS_REG  0x27
#define OUT_X_L     0x28
#define OUT_X_H     0x29
#define OUT_Y_L     0x2A
#define OUT_Y_H     0x2B
#define OUT_Z_L     0x2C
#define OUT_Z_H     0x2D
#define TEMP_OUT_L  0x2E
#define TEMP_OUT_H  0x2F
#define INT_CFG     0x30
#define INT_SRC     0x31
#define INT_THS_L   0x32
#define INT_THS_H   0x33

/* CTRL_CFG2 bit field */
#define CTRL_REG2_FS0   (1<<5)
#define CTRL_REG2_FS1   (1<<6)

/* INT_CFG bit field */
#define INT_CFG_IEN (1<<0)
#define INT_CFG_LIR (1<<1)
#define INT_CFG_IEA (1<<2)
#define INT_CFG_BIT4    (1<<3)
#define INT_CFG_ZIEA    (1<<5)
#define INT_CFG_YIEA    (1<<6)
#define INT_CFG_XIEA    (1<<7)

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

#endif
