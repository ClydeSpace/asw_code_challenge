#include "i2c.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// chance of successful communication; this simulates a communication failure
const float SUCCESS_CHANCE = 0.8;

float floatRand() {
    return (float)rand() / (float)RAND_MAX;
}

status_t i2c_read_byte(const uint8_t bus_address, const uint8_t register_address, uint8_t *byte) {
    if (floatRand() < SUCCESS_CHANCE) {
        printf("read [%d] from bus [%d] for register [%d]\n", 0xff, bus_address, register_address);
        *byte = 0xff;
        return STATUS_OK;
    }
    printf("Failed to read!\n");
    return STATUS_ERROR;
}

status_t i2c_read_bytes(
    const uint8_t bus_address,
    const uint8_t register_address,
    const uint16_t length,
    uint8_t *buffer) {
    if (floatRand() < SUCCESS_CHANCE) {
        printf("read [%d] bytes from bus [%d] for register [%d]\n", length, bus_address, register_address);
        /* Setting the output to some arbitrary value */
        for (size_t i = 0; i < length; ++i) {
            buffer[i] = 0xff - i;
        }
        return STATUS_OK;
    }
    printf("Failed to read!\n");
    return STATUS_ERROR;
}

status_t i2c_write_byte(const uint8_t bus_address, const uint8_t register_address, const uint8_t byte) {
    if (floatRand() < SUCCESS_CHANCE) {
        printf("write [%d] to bus [%d] for register [%d]\n", byte, bus_address, register_address);
        return STATUS_OK;
    }
    printf("Failed to write!\n");
    return STATUS_ERROR;
}

status_t i2c_write_bytes(
    const uint8_t bus_address,
    const uint8_t register_address,
    const uint16_t length,
    const uint8_t *buffer) {
    if (floatRand() < SUCCESS_CHANCE) {
        printf("write [%d] bytes to bus [%d] for register [%d]\n", length, bus_address, register_address);
        for (size_t i = 0; i < length; ++i) {
            printf("%p", buffer);
        }
        printf("\n");
        return STATUS_OK;
    }
    return STATUS_ERROR;
}
