#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stdint.h>

// Define the structure to hold full-scale configuration information
typedef struct {
    uint8_t raw_value;   
    int16_t range_in_gauss;
} lis3mdl_full_scale_t;

// Define the structure to hold output data rate configuration information
typedef struct {
    uint8_t raw_value;  
    uint16_t rate_in_hz;  
} lis3mdl_output_data_rate_t;

// Define the structure to hold interrupt configuration information
typedef struct {
    uint8_t enable_x_axis; // Whether to enable interrupt generation on the X-axis (0: disable, 1: enable)
    uint8_t enable_y_axis; // Whether to enable interrupt generation on the Y-axis (0: disable, 1: enable)
    uint8_t enable_z_axis; // Whether to enable interrupt generation on the Z-axis (0: disable, 1: enable)
} lis3mdl_interrupt_config_t;

// Define a structure to hold the axis data
typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} lis3mdl_axis_data_t;

#endif /* DATA_STRUCTURES_H */