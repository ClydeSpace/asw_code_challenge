#include "i2c.h"
#include <stdio.h>

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

/* i2c Device name. Need to check the device name in the system before testing */
#define I2C_DEV "/dev/i2c-0"
/* Device I2C address */
#define DEV_I2C_ADDRESS 0x1
#define I2C_SLAVE_READ  0x39
#define I2C_SLAVE_WRITE 0x38

/* Register addresses are referenced from the datasheet document. */
#define CTRL_REG1_ADDRESS 0x20 /* Register address for the output data rate */
#define CTRL_REG2_ADDRESS 0x21 /* Full-scale configuration */
#define OUT_X_L_ADDRESS   0x28 /* x axis data output length */
#define OUT_X_H_ADDRESS   0x29 /* x axis data output height */
#define OUT_Y_L_ADDRESS   0x2A /* y axis data ouput length */
#define OUT_Y_H_ADDRESS   0x2B /* y axis data output height */
#define OUT_Z_L_ADDRESS   0x2C /* z axis data output length */
#define OUT_Z_H_ADDRESS   0x2D /* z axis data output height */
#define INT_CFG_ADDRESS   0x30 /* Enable / Disable interrupt */

status_t i2c_read(uint8_t  bus_address,
                  uint8_t  register_address,
                  uint16_t length,
                  uint8_t  *buffer) {

    int      fd;
    uint8_t  read_address = register_address;

    /* Open the I2C bus */
    if ((fd = open(I2C_DEV, O_RDWR)) < 0) {
        printf("Failed to open I2C bus");
        return STATUS_ERROR;
    }

    /* Set the slave address */
    if (ioctl(fd, I2C_SLAVE_READ, bus_address) < 0) {
        printf("Failed to set I2C slave address");
        close(fd);
        return STATUS_ERROR;
    }

    /* Write register address to the device */
    if (write(fd, &read_address, 1) != 1) {
        printf("Failed to write register address to I2C device");
        close(fd);
        return STATUS_ERROR;
    }

    /* Read data from the device*/
    if (read(fd, buffer, length) != length) {
        printf("Failed to read from I2C device");
        close(fd);
        return STATUS_ERROR;
    }

    /* Close the I2C bus*/
    close(fd);

    printf("Read [%d] bytes from bus [%d] for register [%d]\n", length, bus_address, register_address);
    return STATUS_OK;
}

status_t i2c_write(uint8_t bus_address, uint8_t register_address, uint16_t length, uint8_t *buffer) {

    int fd;

    /* Open the I2C bus */
    if ((fd = open(I2C_DEV, O_RDWR)) < 0) {
        printf("Failed to open I2C bus");
        return STATUS_ERROR;
    }

    /* Set the slave address */
    if (ioctl(fd, I2C_SLAVE_WRITE, bus_address) < 0) {
        printf("Failed to set I2C slave address");
        close(fd);
        return STATUS_ERROR;
    }

    /*  Write register address and data to the device */
    uint8_t write_buffer[length + 1];
    write_buffer[0] = register_address;
    for (int i = 0; i < length; ++i) {
        write_buffer[i + 1] = buffer[i];
    }

    if (write(fd, write_buffer, length + 1) != length + 1) {
        printf("Failed to write to I2C device");
        close(fd);
        return STATUS_ERROR;
    }

    /* Close the I2C bus */
    close(fd);

    printf("Write [%d] bytes to bus [%d] for register [%d]\n", length, bus_address, register_address);
    return STATUS_OK;
}

/*Get the full-scale configuration*/
status_t Get_full_scale_config(uint8_t *buffer, uint16_t length) {
    return i2c_read(DEV_I2C_ADDRESS, CTRL_REG2_ADDRESS, length, buffer);
}

/*Get the output data rate*/
status_t Get_output_data_rate(uint8_t *output_data_rate, uint8_t *buffer, uint16_t length) {

    /* Perform I2C read operation to get CTRL_REG1 value */
    status_t read_status = i2c_read(DEV_I2C_ADDRESS, CTRL_REG1_ADDRESS, length, buffer);

    if (read_status != STATUS_OK) {
        /* Return if I2C read fails */
        return read_status;
    }

    /* Extract DO2, DO1, DO0 bits */
    *output_data_rate = (*buffer >> 3) & 0x07;

    return STATUS_OK;
}

/*set the output data rate*/
status_t Set_output_data_rate(uint8_t rate) {

    /* Read the current value of CTRL_REG1 register */
    uint8_t current_value;
    status_t read_status = i2c_read(DEV_I2C_ADDRESS, CTRL_REG1_ADDRESS, sizeof(current_value), &current_value);

    if (read_status != STATUS_OK) {
        /* Return if I2C read fails */
        return read_status;
    }

    /* Update the output data rate bits in the current value */

    /* Clear existing DO2, DO1, DO0 bits */
    current_value &= ~(0x07 << 3);

     /* Set new DO2, DO1, DO0 bits with the provided rate */
    current_value |= (rate & 0x07) << 3;

    /* Write the modified value back to the CTRL_REG1 register */
    return i2c_write(DEV_I2C_ADDRESS, CTRL_REG1_ADDRESS, sizeof(current_value), &current_value);
}

/*Enable the interrupt pin*/
status_t Enable_interrupt_pin() {

    /* Read the current value of INT_CFG register */
    uint8_t current_value;
    status_t read_status = i2c_read(DEV_I2C_ADDRESS, INT_CFG_ADDRESS, sizeof(current_value), &current_value);

    if (read_status != STATUS_OK) {
        /* Return if I2C read fails */
        return read_status;
    }

    /* Set the IEN bit to enable the interrupt pin */
    current_value |= (1 << 7);

    /* Write the modified value back to the INT_CFG register */
    return i2c_write(DEV_I2C_ADDRESS, INT_CFG_ADDRESS, sizeof(current_value), &current_value);
}

/*Disable the interrupt pin*/
status_t Disable_interrupt_pin() {

    /* Read the current value of INT_CFG register */
    uint8_t current_value;
    status_t read_status = i2c_read(DEV_I2C_ADDRESS, INT_CFG_ADDRESS, sizeof(current_value), &current_value);

    if (read_status != STATUS_OK) {
        return read_status; // Return if I2C read fails
    }

    /*  Clear the IEN bit to disable the interrupt pin */
    current_value &= ~(1 << 7);

    /* Write the modified value back to the INT_CFG register */
    return i2c_write(DEV_I2C_ADDRESS, INT_CFG_ADDRESS, sizeof(current_value), &current_value);
}

/*Read the output data of a specified axis*/
status_t Read_output_data(uint8_t *buffer, uint16_t length, uint8_t axis_address_length, uint8_t axis_address_height) {

    status_t status_length = i2c_read(DEV_I2C_ADDRESS, axis_address_length, length, buffer);

    if (status_length != STATUS_OK) {
        /* Return error if reading length byte fails */
        return status_length;
    }

    status_t status_height = i2c_read(DEV_I2C_ADDRESS, axis_address_height, length, buffer + 1);
    if (status_height != STATUS_OK) {
        /* Return error if reading length byte fails */
        return status_height;
    }

    return STATUS_OK;
}

int main() {

    int16_t  axis_value;
    uint8_t  buffer[1]; /* Buffer for single-byte data */
    uint8_t  axis_data[2]; /* 2 bytes for axis data (length and height) */
    uint8_t  new_rate = 0x64; /* Added for example */
    uint8_t  output_data_rate;     /* Output data rate variable  */
    status_t status;

    /* Get full-scale configuration */
    status = Get_full_scale_config(buffer , 1);
    if (status == STATUS_OK) {
        printf("Full-scale configuration: 0x%02X\n", buffer[0]);
    } else {
        printf("Failed to get full-scale configuration.\n");
    }

    /* Get output data rate */
    status = Get_output_data_rate(&output_data_rate,buffer, sizeof(buffer));
    if (status == STATUS_OK) {
        printf("Output data rate: 0x%02X\n", buffer[0]);
    } else {
        printf("Failed to get output data rate.\n");
    }

    /* Set output data rate */
    status = Set_output_data_rate(new_rate);
    if (status == STATUS_OK) {
        printf("Output data rate set successfully.\n");
    } else {
        printf("Failed to set output data rate.\n");
    }

    /* Enable interrupt pin */
    status = Enable_interrupt_pin();
    if (status == STATUS_OK) {
        printf("Interrupt pin enabled.\n");
    } else {
        printf("Failed to enable interrupt pin.\n");
    }

    /* Disable interrupt pin */
    status = Disable_interrupt_pin();
    if (status == STATUS_OK) {
        printf("Interrupt pin disabled.\n");
    } else {
        printf("Failed to disable interrupt pin.\n");
    }

    /* Read X-axis data */
    status = Read_output_data(axis_data, sizeof(axis_data), OUT_X_L_ADDRESS, OUT_X_H_ADDRESS);
    if (status == STATUS_OK) {
        axis_value = (axis_data[1] << 8) | axis_data[0];
        printf("Output data (X-axis): %d\n", axis_value);
    } else {
        printf("Failed to read X-axis data.\n");
    }

    /* Read Y-axis data */
    status = Read_output_data(axis_data, sizeof(axis_data), OUT_Y_L_ADDRESS, OUT_Y_H_ADDRESS);
    if (status == STATUS_OK) {
        axis_value = (axis_data[1] << 8) | axis_data[0];
        printf("Output data (Y-axis): %d\n", axis_value);
    } else {
        printf("Failed to read Y-axis data.\n");
    }

    /* Read Z-axis data */
    status = Read_output_data(axis_data, sizeof(axis_data), OUT_Z_L_ADDRESS, OUT_Z_H_ADDRESS);
    if (status == STATUS_OK) {
        axis_value = (axis_data[1] << 8) | axis_data[0];
        printf("Output data (Z-axis): %d\n", axis_value);
    } else {
        printf("Failed to read Z-axis data.\n");
    }

    return 0;
}