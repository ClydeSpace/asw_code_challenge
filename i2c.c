#include "i2c.h"

#include <stdint.h>
#include <stdio.h>

#define BUS_NO  0x00

status_t i2c_read(
    uint8_t bus_address,
    uint8_t register_address,
    uint16_t length,
    uint8_t *buffer)
{
    printf(
        "read [%d] bytes from bus [%d] for register [%d]\n",
        length,
        bus_address,
        register_address);

    /* Setting the output to some arbitrary value */
    for (size_t i = 0; i < length; ++i) {
        buffer[i] = 0xff;
    }

    return STATUS_OK;
}

status_t i2c_write(
    uint8_t bus_address,
    uint8_t register_address,
    uint16_t length,
    uint8_t *buffer)
{
    printf(
        "write [%d] bytes to bus [%d] for register [%d]\n\t",
        length,
        bus_address,
        register_address);

    for (size_t i = 0; i < length; ++i) {
        printf("%p", buffer);
    }
    printf("\n");
    return STATUS_OK;
}

/*
 * Get the full-scale configuration
 */
status_t get_full_scale_configuration()
{
    status_t res;
    uint8_t buffer[1];

    res = i2c_read(BUS_NO, CTRL_REG2, 1, buffer);
    if (res == STATUS_OK) {
        uint8_t full_scale_conf = (buffer[0] & (CTRL_REG2_FS1 | CTRL_REG2_FS0)) >> 6;
        printf("Full-scale configuration: ");
        switch(full_scale_conf) {
        case 0x00:
            printf("+/-4 gauss\n");
            break;
        case 0x01:
            printf("+/-8 gauss\n");
            break;
        case 0x02:
            printf("+/-12 gauss\n");
            break;
        case 0x03:
            printf("+/-16 gauss\n");
            break;
        }
        return STATUS_OK;
    }

    return STATUS_ERROR;
}

/*
 * Get the output data rate
 */
status_t get_output_data_rate()
{
    char buffer[1];
    status_t res = i2c_read(BUS_NO, CTRL_REG1, 1, buffer);

    if (res == STATUS_OK) {
        size_t data_rate_code = (buffer[0] >> 2) & 0x7;
        printf("Output data rate: ");
        switch(data_rate_code) {
        case 0x00:
            printf("%f\n", 0.625);
            break;
        case 0x01:
            printf("%f\n", 1.25);
            break;
        case 0x02:
            printf("%f\n", 2.5);
            break;
        case 0x03:
            printf("%d\n", 5);
            break;
        case 0x04:
            printf("%d\n", 10);
            break;
        case 0x05:
            printf("%d\n", 20);
            break;
        case 0x06:
            printf("%d\n", 40);
            break;
        case 0x07:
            printf("%d\n", 80);
            break;
        }

        return STATUS_OK;
    }

    return STATUS_ERROR;
}

/*
 * Set the data rate
 */
status_t set_output_data_rate(float rate)
{
    size_t data_rate_code = 0x00;
    if (rate == 0.625) data_rate_code = 0x00;
    else if (rate == 1.25) data_rate_code = 0x01;
    else if (rate == 2.5) data_rate_code = 0x02;
    else if (rate == 5) data_rate_code = 0x03;
    else if (rate == 10) data_rate_code = 0x04;
    else if (rate == 20) data_rate_code = 0x05;
    else if (rate == 40) data_rate_code = 0x06;
    else if (rate == 80) data_rate_code = 0x07;
    else {
        printf("A rate not supported.\n");
        return STATUS_ERROR;
    }

    char buffer[1];
    status_t res = i2c_read(BUS_NO, CTRL_REG1, 1, buffer);

    if (res == STATUS_OK) {
        buffer[0] = (buffer[0] & 0x1C) | (data_rate_code << 2);
        res = i2c_write(BUS_NO, CTRL_REG1, 1, buffer);
        if (res == STATUS_OK) {
            printf("Setting the output data rate to %f\n", rate);
            return STATUS_OK;
        }
    }

    return STATUS_ERROR;
}

/*
 * Enable the interrupt pin
 */
status_t enable_irq()
{
    char buffer[1];
    status_t res = i2c_read(BUS_NO, INT_CFG, 1, buffer);

    if (res == STATUS_OK) {
        buffer[0] = (buffer[0] & INT_CFG_BIT4) | INT_CFG_XIEA | INT_CFG_YIEA |
                    INT_CFG_ZIEA | INT_CFG_BIT4 | INT_CFG_IEN;
        res = i2c_write(BUS_NO, INT_CFG, 1, buffer);
        if (res == STATUS_OK) {
            printf("Interrupt pin is enabled.\n");
            return STATUS_OK;
        }
    }

    return STATUS_ERROR;
}

/*
 * Disable the interrupt pin
 */
status_t disable_irq()
{
    char buffer[1];
    status_t res = i2c_read(BUS_NO, INT_CFG, 1, buffer);

    if (res == STATUS_OK) {
        buffer[0] = buffer[0] & (~INT_CFG_IEN);
        res = i2c_write(BUS_NO, INT_CFG, 1, buffer);
        if (res == STATUS_OK) {
            printf("Interrupt pin is disable.\n");
            return STATUS_OK;
        }
    }

    return STATUS_ERROR;
}

/*
 * Read the output data of a specified axis
 */
status_t get_axis_data(char axis)
{
    if (axis != 'x' && axis != 'X' &&
        axis != 'y' && axis != 'Y' &&
        axis != 'z' && axis != 'Z' ) {
            printf("Invalid axis.\n");
            return STATUS_ERROR;
    }

    size_t reg_addr;
    switch(axis) {
    case 'x':
    case 'X':
        reg_addr = OFFSET_X_REG_L_M;
        break;
    case 'y':
    case 'Y':
        reg_addr = OFFSET_Y_REG_L_M;
        break;
    case 'z':
    case 'Z':
        reg_addr = OFFSET_Z_REG_L_M;
        break;
    }

    uint8_t buffer[2];
    status_t res = i2c_read(BUS_NO, reg_addr, 2, buffer);
    if (res == STATUS_OK) {
        uint16_t data = (buffer[1] << 8) | buffer[0];
        printf("The data of axis %c is: 0x%x\n", axis, data);
        return STATUS_OK;
    }

    return STATUS_ERROR;
}

int main(int argc, char *argv[])
{
    get_full_scale_configuration();
    get_output_data_rate();
    set_output_data_rate(2.5);
    enable_irq();
    disable_irq();
    get_axis_data('x');

    return 0;
}