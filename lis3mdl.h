#ifndef LIS3MDL_H
#define LIS3MDL_H

#include <stdbool.h> // Include for using the 'bool' type
#include "comm_interface.h"



// Register map definitions
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define STATUS_REG 0x27
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D
#define INT_CFG 0x30
#define INT_SRC 0x31
#define INT_THS_L 0x32
#define INT_THS_H 0x33

/**
 * @brief Error codes for LIS3MDL operations.
 */
typedef enum {
    LIS3MDL_OK = 0, ///< Operation successful.
    LIS3MDL_ERR_CONFIG, ///< Communication interface not configured.
    LIS3MDL_ERR_PARAM, ///< Invalid parameter.
    LIS3MDL_ERR_COMM, ///< Communication error.
    // Add other error codes as needed based on the driver's requirements.
} lis3mdl_err_t;


// Function prototypes for sensor operations
lis3mdl_err_t lis3mdl_get_full_scale_configuration(uint8_t *fs_config);
lis3mdl_err_t lis3mdl_get_output_data_rate(uint8_t *odr);
lis3mdl_err_t lis3mdl_set_output_data_rate(uint8_t odr);
lis3mdl_err_t lis3mdl_set_interrupt_pin_enable(bool enable);
lis3mdl_err_t lis3mdl_read_axis_data(char axis, int16_t *data);
void lis3mdl_init_comm_interface(comm_interface_t *interface_ptr);

#endif // LIS3MDL_H
