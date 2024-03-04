#include "lis3mdl.h"
#include <stddef.h>

/** @file
 *  @brief LIS3MDL magnetometer driver using a generic communication interface.
 */

/** 
 * @var comm_interface
 * @brief Global variable to store the communication interface.
 */
comm_interface_t *comm_interface = NULL;

/**
 * @brief Initializes the LIS3MDL driver with a communication interface.
 * 
 * This function sets the global communication interface to the one provided
 * by the caller. It must be called before any other LIS3MDL functions to 
 * ensure proper operation of the driver.
 * 
 * @param interface_ptr Pointer to the communication interface.
 */
void lis3mdl_init_comm_interface(comm_interface_t *interface_ptr) {
    comm_interface = interface_ptr;
    comm_interface->init();
}

/**
 * @brief Gets the full-scale configuration of the LIS3MDL sensor.
 * 
 * Reads the full-scale configuration from the CTRL_REG2 register of the LIS3MDL.
 * The full-scale configuration determines the range of magnetic field values the
 * sensor can measure.
 * 
 * @param fs_config Pointer to store the full-scale configuration value.
 * @return lis3mdl_err_t Error code indicating the status of the operation.
 */
lis3mdl_err_t lis3mdl_get_full_scale_configuration(uint8_t *fs_config) {
    if (!comm_interface) return LIS3MDL_ERR_CONFIG;
    if (!fs_config) return LIS3MDL_ERR_PARAM;
    if (comm_interface->read(comm_interface->config.dev_addr, CTRL_REG2,1, fs_config) != COMM_SUCCESS) {
        return LIS3MDL_ERR_COMM;
    }
    *fs_config &= 0x60;
    return LIS3MDL_OK;
}

/**
 * @brief Gets the output data rate of the LIS3MDL sensor.
 * 
 * Reads the output data rate configuration from the CTRL_REG1 register.
 * The output data rate determines how frequently the sensor updates its
 * measurements.
 * 
 * @param odr Pointer to store the output data rate value.
 * @return lis3mdl_err_t Error code indicating the status of the operation.
 */
lis3mdl_err_t lis3mdl_get_output_data_rate(uint8_t *odr) {
    if (!comm_interface) return LIS3MDL_ERR_CONFIG;
    if (!odr) return LIS3MDL_ERR_PARAM;
    uint8_t reg_val;
    if (comm_interface->read(comm_interface->config.dev_addr, CTRL_REG1,1, &reg_val) != COMM_SUCCESS) {
        return LIS3MDL_ERR_COMM;
    }
    *odr = reg_val & 0x1C;
    return LIS3MDL_OK;
}

/**
 * @brief Sets the output data rate of the LIS3MDL sensor.
 * 
 * Writes a new output data rate configuration to the CTRL_REG1 register.
 * 
 * @param odr The desired output data rate value, masked to valid bits.
 * @return lis3mdl_err_t Error code indicating the status of the operation.
 */
lis3mdl_err_t lis3mdl_set_output_data_rate(uint8_t odr) {
    if (!comm_interface) return LIS3MDL_ERR_CONFIG;
    uint8_t reg_val = odr & 0x1C;
    if (comm_interface->write(comm_interface->config.dev_addr, CTRL_REG1, 1, &reg_val) != COMM_SUCCESS) {
        return LIS3MDL_ERR_COMM;
    }
    return LIS3MDL_OK;
}

/**
 * @brief Enables or disables the interrupt pin of the LIS3MDL sensor.
 * 
 * Configures the INT_CFG register to enable or disable the sensor's interrupt pin.
 * 
 * @param enable True to enable the interrupt pin, false to disable it.
 * @return lis3mdl_err_t Error code indicating the status of the operation.
 */
lis3mdl_err_t lis3mdl_set_interrupt_pin_enable(bool enable) {
    if (!comm_interface) return LIS3MDL_ERR_CONFIG;
    uint8_t reg_val = enable ? 0x01 : 0x00;
    if (comm_interface->write(comm_interface->config.dev_addr, INT_CFG, 1 , &reg_val) != COMM_SUCCESS) {
        return LIS3MDL_ERR_COMM;
    }
    return LIS3MDL_OK;
}

/**
 * @brief Reads magnetic data for a specified axis from the LIS3MDL sensor.
 * 
 * Reads the magnetic field data for the x, y, or z axis. This involves reading
 * two registers to obtain the 16-bit value representing the magnetic field
 * measurement for the specified axis.
 * 
 * @param axis The axis ('x', 'y', or 'z') to read from.
 * @param dataPointer to store the 16-bit magnetic data value. Must not be NULL.
 * @return lis3mdl_err_t Error code indicating the status of the operation.
 */
lis3mdl_err_t lis3mdl_read_axis_data(char axis, int16_t *data) {
    if (!comm_interface) return LIS3MDL_ERR_CONFIG; // Ensure communication interface is initialized
    if (!data) return LIS3MDL_ERR_PARAM; // Validate non-NULL data pointer
    uint8_t l_reg, h_reg; // Register addresses for low and high data bytes

    // Determine correct registers based on specified axis
    switch (axis) {
        case 'x': l_reg = OUT_X_L; h_reg = OUT_X_H; break;
        case 'y': l_reg = OUT_Y_L; h_reg = OUT_Y_H; break;
        case 'z': l_reg = OUT_Z_L; h_reg = OUT_Z_H; break;
        default: return LIS3MDL_ERR_PARAM; // Invalid axis parameter
    }

    uint8_t l_data = 0, h_data = 0;
    comm_status_t read_l_status = comm_interface->read(comm_interface->config.dev_addr, l_reg, 1 ,&l_data);
    if (read_l_status != COMM_SUCCESS) {
        return LIS3MDL_ERR_COMM;
    }
    comm_status_t read_h_status = comm_interface->read(comm_interface->config.dev_addr, h_reg, 1 , &h_data);
    if (read_h_status != COMM_SUCCESS) {
        return LIS3MDL_ERR_COMM;
    }
    // Combine low and high bytes into a single 16-bit value
    *data = (int16_t)((h_data << 8) | l_data);

    return LIS3MDL_OK; // Success
}
