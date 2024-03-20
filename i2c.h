#ifndef I2C_HEADER_H
#define I2C_HEADER_H

#include <stdint.h>

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

/*Get the full-scale configuration*/
status_t Get_full_scale_config(uint8_t *buffer,
                               uint16_t length);

/*Get and set the output data rate*/
status_t Get_output_data_rate(uint8_t *output_data_rate,
                              uint8_t *buffer,
                              uint16_t length);
status_t Set_output_data_rate(uint8_t rate);

/*Enable or disable the interrupt pin*/
status_t Enable_interrupt_pin();
status_t Disable_interrupt_pin();

/*Read the output data of a specified axis*/
status_t Read_output_data(uint8_t *buffer,
                          uint16_t length,
                          uint8_t axis_address_length,
                          uint8_t axis_address_height);
#endif