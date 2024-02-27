#include "i2c.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <pthread.h>

#define I2C_DEVICE "/dev/i2c-1"  // Path to the I2C device file

pthread_mutex_t i2c_mutex = PTHREAD_MUTEX_INITIALIZER;

status_t i2c_read(
    uint8_t bus_address,
    uint8_t register_address,
    uint16_t length,
    uint8_t *buffer)
{
    // Lock the mutex to prevent concurrent access to the I2C bus
    pthread_mutex_lock(&i2c_mutex);

    int i2c_fd;

    // Open the I2C device
    if ((i2c_fd = open(I2C_DEVICE, O_RDWR)) < 0) {
        perror("Failed to open I2C device");
        return STATUS_ERROR;
    }

    // Set the I2C slave address
    if (ioctl(i2c_fd, I2C_SLAVE, bus_address) < 0) {
        perror("Failed to set I2C slave address");
        close(i2c_fd);
        return STATUS_ERROR;
    }

    // Write the register address to the device to specify where to start reading from
    if (write(i2c_fd, &register_address, 1) != 1) {
        perror("Failed to write register address to I2C device");
        close(i2c_fd);
        return STATUS_ERROR;
    }

    // Read data from the device
    if (read(i2c_fd, buffer, length) != length) {
        perror("Failed to read data from I2C device");
        close(i2c_fd);
        return STATUS_ERROR;
    }

    // Close the I2C device
    close(i2c_fd);

    // Unlock the mutex to allow other threads to access the I2C bus
    pthread_mutex_unlock(&i2c_mutex);
    return STATUS_OK;
}

status_t i2c_write(
    uint8_t bus_address,
    uint8_t register_address,
    uint16_t length,
    uint8_t *buffer)
{
    // Lock the mutex to prevent concurrent access to the I2C bus
    pthread_mutex_lock(&i2c_mutex);

    int i2c_fd;

    // Open the I2C device
    if ((i2c_fd = open(I2C_DEVICE, O_RDWR)) < 0) {
        perror("Failed to open I2C device");
        return STATUS_ERROR;
    }

    // Set the I2C slave address
    if (ioctl(i2c_fd, I2C_SLAVE, bus_address) < 0) {
        perror("Failed to set I2C slave address");
        close(i2c_fd);
        return STATUS_ERROR;
    }

    // Write the register address to the device to specify where to start writing
    if (write(i2c_fd, &register_address, 1) != 1) {
        perror("Failed to write register address to I2C device");
        close(i2c_fd);
        return STATUS_ERROR;
    }

    // Write data to the device
    if (write(i2c_fd, buffer, length) != length) {
        perror("Failed to write data to I2C device");
        close(i2c_fd);
        return STATUS_ERROR;
    }

    // Close the I2C device
    close(i2c_fd);

    // Unlock the mutex to allow other threads to access the I2C bus
    pthread_mutex_unlock(&i2c_mutex);
    return STATUS_OK;
}

// Function to initialize the LIS3MDL sensor with default values
status_t lis3mdl_init(uint8_t bus_address) {
    // Check WHO_AM_I register to verify device presence
    uint8_t who_am_i;
    if (i2c_read(bus_address, LIS3MDL_REG_WHO_AM_I, 1, &who_am_i) != STATUS_OK) {
        printf("Failed to read WHO_AM_I register\n");
        return STATUS_ERROR;
    }

    if (who_am_i != 0x3D) {
        printf("Unexpected WHO_AM_I value: 0x%X\n", who_am_i);
        return STATUS_ERROR;
    }

    // Configure CTRL1 register with default values
    uint8_t ctrl_reg1_data = DEFAULT_CTRL_REG1_VALUE;
    if (i2c_write(bus_address, CTRL_REG1, 1, &ctrl_reg1_data) != STATUS_OK) {
        printf("Failed to write CTRL_REG1 register\n");
        return STATUS_ERROR;
    }

    return STATUS_OK;
}

// Function to get the full-scale configuration
status_t lis3mdl_get_full_scale(uint8_t bus_address, lis3mdl_full_scale_t *full_scale) {
    // Read the register containing the full-scale configuration
    uint8_t raw_value;
    if (i2c_read(bus_address, CTRL_REG2, 1, &raw_value) != STATUS_OK) {
        printf("Failed to read full-scale configuration register\n");
        return STATUS_ERROR;
    }

    // Decode the raw value and assign the range accordingly
    switch (raw_value & 0x60) {
        case 0x00:
            full_scale->range_in_gauss = 4000; // ±4 gauss
            break;
        case 0x20:
            full_scale->range_in_gauss = 8000; // ±8 gauss
            break;
        case 0x40:
            full_scale->range_in_gauss = 12000; // ±12 gauss
            break;
        case 0x60:
            full_scale->range_in_gauss = 16000; // ±16 gauss
            break;
        default:
            printf("Invalid full-scale configuration value\n");
            return STATUS_ERROR;
    }

    // Store the raw value for reference
    full_scale->raw_value = raw_value;

    return STATUS_OK;
}

// Function to get the output data rate
status_t lis3mdl_get_output_data_rate(uint8_t bus_address, lis3mdl_output_data_rate_t *output_data_rate) {
    // Read the register containing the output data rate configuration
    uint8_t raw_value;
    if (i2c_read(bus_address, CTRL_REG1, 1, &raw_value) != STATUS_OK) {
        printf("Failed to read output data rate register\n");
        return STATUS_ERROR;
    }

    // Decode the raw value and assign the rate accordingly
    switch (raw_value & 0x07) {
        case 0x00:
            output_data_rate->rate_in_hz = 0.625;
            break;
        case 0x01:
            output_data_rate->rate_in_hz = 1.25;
            break;
        case 0x02:
            output_data_rate->rate_in_hz = 2.5;
            break;
        case 0x03:
            output_data_rate->rate_in_hz = 5;
            break;
        case 0x04:
            output_data_rate->rate_in_hz = 10;
            break;
        case 0x05:
            output_data_rate->rate_in_hz = 20;
            break;
        case 0x06:
            output_data_rate->rate_in_hz = 40;
            break;
        case 0x07:
            output_data_rate->rate_in_hz = 80;
            break;
        default:
            printf("Invalid output data rate configuration value\n");
            return STATUS_ERROR;
    }

    // Store the raw value for reference
    output_data_rate->raw_value = raw_value;

    return STATUS_OK;
}

// Function to set the output data rate
status_t lis3mdl_set_output_data_rate(uint8_t bus_address, uint16_t output_data_rate_hz) {
    // Determine the appropriate value to write based on the desired output data rate
    uint8_t output_data_rate_value;

    // Select the closest valid output data rate based on the desired rate
    if (output_data_rate_hz <= 0.625) {
        // Set 0.625Hz
        output_data_rate_value = 0x00;
    } else if (output_data_rate_hz <= 1.25) {
        // Set 1.25Hz
        output_data_rate_value = 0x01;
    } else if (output_data_rate_hz <= 2.5) {
        // Set 2.5Hz
        output_data_rate_value = 0x02;
    } else if (output_data_rate_hz <= 5) {
        // Set 5Hz
        output_data_rate_value = 0x03;
    } else if (output_data_rate_hz <= 10) {
        // Set 10Hz
        output_data_rate_value = 0x04;
    } else if (output_data_rate_hz <= 20) {
        // Set 20Hz
        output_data_rate_value = 0x05;
    } else if (output_data_rate_hz <= 40) {
        // Set 40Hz
        output_data_rate_value = 0x06;
    } else {
        // Set 80Hz
        output_data_rate_value = 0x07; // Maximum supported rate
    }

    if (i2c_write(bus_address, CTRL_REG1, 1, &output_data_rate_value) != STATUS_OK) {
        printf("Failed to write output data rate register\n");
        return STATUS_ERROR;
    }

    return STATUS_OK;
}

// Function to enable or disable interrupt pin for specified axes
status_t lis3mdl_configure_interrupt(uint8_t bus_address, lis3mdl_interrupt_config_t *interrupt_config) {
    // Read the current interrupt configuration
    uint8_t current_interrupt_config;
    if (i2c_read(bus_address, INT_CFG, 1, &current_interrupt_config) != STATUS_OK) {
        fprintf(stderr, "Failed to read interrupt configuration register\n");
        return STATUS_ERROR;
    }

    // Modify the interrupt configuration based on the specified axes to enable or disable
    if (interrupt_config->enable_x_axis) {
        current_interrupt_config |= (1 << 2); // Enable X-axis interrupt
    } else {
        current_interrupt_config &= ~(1 << 2); // Disable X-axis interrupt
    }
    if (interrupt_config->enable_y_axis) {
        current_interrupt_config |= (1 << 1); // Enable Y-axis interrupt
    } else {
        current_interrupt_config &= ~(1 << 1); // Disable Y-axis interrupt
    }
    if (interrupt_config->enable_z_axis) {
        current_interrupt_config |= (1 << 0); // Enable Z-axis interrupt
    } else {
        current_interrupt_config &= ~(1 << 0); // Disable Z-axis interrupt
    }

    // Write the modified interrupt configuration back to the register
    if (i2c_write(bus_address, INT_CFG, 1, &current_interrupt_config) != STATUS_OK) {
        printf("Failed to write interrupt configuration register\n");
        return STATUS_ERROR;
    }

    return STATUS_OK;
}

// Function to read the output data of a specified axis
status_t lis3mdl_read_axis_data(uint8_t bus_address, lis3mdl_axis_data_t *axis_data) {
    // Define the register addresses for the X, Y, and Z axes' data output registers
    const uint8_t axis_data_registers[3][2] = {
        {OUT_X_L, OUT_X_H},
        {OUT_Y_L, OUT_Y_H}, 
        {OUT_Z_L, OUT_Z_H}  
    };

    // Iterate over each axis and read the appropriate register(s) to get the data
    for (int axis = 0; axis < 3; axis++) {
        // Read the appropriate register(s) to get the data for the current axis
        int16_t axis_value;
        if (i2c_read(bus_address, axis_data_registers[axis][0], 2, (uint8_t *)&axis_value) != STATUS_OK) {
            printf("Failed to read axis data\n");
            return STATUS_ERROR;
        }

        // Swap bytes to convert from little-endian to big-endian if needed
        axis_value = (axis_value >> 8) | (axis_value << 8);

        switch (axis) {
            case 0: // X-axis
                axis_data->x = axis_value;
                break;
            case 1: // Y-axis
                axis_data->y = axis_value;
                break;
            case 2: // Z-axis
                axis_data->z = axis_value;
                break;
            default:
                break;
        }
    }

    return STATUS_OK;
}
