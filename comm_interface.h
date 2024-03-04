#ifndef COMM_INTERFACE_H
#define COMM_INTERFACE_H

#include <stdint.h>

// Define comm_status_t if not already defined
typedef enum {
    COMM_SUCCESS = 0,
    COMM_ERROR = 1
} comm_status_t;

typedef struct {
    uint8_t dev_addr;
} comm_config_t;

// Adjusted interface for both I2C and SPI
typedef comm_status_t (*comm_read_t)(uint8_t dev_addr, uint8_t reg_addr, uint16_t length ,uint8_t *data );
typedef comm_status_t (*comm_write_t)(uint8_t dev_addr, uint8_t reg_addr, uint16_t length , uint8_t *data );


typedef comm_status_t (*comm_int_t)(void);

typedef struct {
    comm_int_t init;
    comm_read_t read;
    comm_write_t write;
    comm_config_t config;
} comm_interface_t;

#endif /* COMM_INTERFACE_H */
