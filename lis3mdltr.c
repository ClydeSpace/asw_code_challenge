#include "lis3mdltr.h"

#include <stdint.h>
#include <stdio.h>

bool get_full_scale_config(uint8_t *scale)
{
    status_t result = i2c_read(
        I2C_ADDR_LOW, 
        REG_CTRL_REG2, 
        1, 
        scale);

    if (result == STATUS_OK)
    {
        // shift 5 places to right to get FS[1:0]
        // no masking required as the MSB bits are already 0
        *scale >>= 5;

        return true;
    }

    return false;
}

float get_sensitivity()
{
    uint8_t scale;
    if(get_full_scale_config(&scale) == STATUS_OK)
    {
        // assigning sensitivity based on full scale config
        switch(scale)
        {
            case 0b00: sensitivity = 6842.0f; break;
            case 0b01: sensitivity = 3421.0f; break;
            case 0b10: sensitivity = 2281.0f; break;
            case 0b11: sensitivity = 1711.0f; break;
        }

        return sensitivity;
    }

    return 0.0f;
}

bool get_output_data_rate(
    uint8_t *op_mode, 
    uint8_t *data_rate, 
    uint8_t *fast_odr)
{
    status_t result = i2c_read(
        I2C_ADDR_LOW, 
        REG_CTRL_REG1, 
        1, 
        buffer);

    if (result == STATUS_OK)
    {
        // shift bits right and extracting OM[1:0], DO[2:0] & FAST_ODR
        *op_mode = buffer[0] >> 5 & 0x3;
        *data_rate = buffer[0] >> 2 & 0x7;
        *fast_odr = buffer[0] >> 1 & 0x1;

        return true;
    }

    return false;
}

bool get_data_x_axis(float *value)
{
    uint8_t status;    
    // check if there is new data available for x-axis
    status_t result = i2c_read(
        I2C_ADDR_LOW, 
        REG_STATUS_REG, 
        1, 
        &status);

    if (result == STATUS_OK)
    {
        // Check the 8th bit of STATUS_REG
        if (status & 0x01)
        {
            // Read the L & H registers (2 bytes)
            status_t result = i2c_read(
                I2C_ADDR_LOW, 
                REG_OUT_X_L, 
                2, 
                buffer);

            if (result == STATUS_OK)
            {
                // Combine L and H register values
                int16_t axis_value = (int16_t) buffer[1] << 8 | buffer[0];
                // Calculate axis value considering sensitivity
                *value = (float) axis_value / get_sensitivity() * 100.0f;

                return true;
            }

            return false;
        }

        return false;
    }    

    return false;
}

bool get_data_y_axis(float *value)
{
    uint8_t status;    
    // check if there is new data available for y-axis
    status_t result = i2c_read(
        I2C_ADDR_LOW, 
        REG_STATUS_REG, 
        1, 
        &status);

    if (result == STATUS_OK)
    {
        // Check the 7th bit of STATUS_REG
        if (status & 0x02)
        {
            // Read the L & H registers (2 bytes)
            status_t result = i2c_read(
                I2C_ADDR_LOW, 
                REG_OUT_Y_L, 
                2, 
                buffer);

            if (result == STATUS_OK)
            {
                // Combine L and H register values
                int16_t axis_value = (int16_t) buffer[1] << 8 | buffer[0];
                // Calculate axis value considering sensitivity
                *value = (float) axis_value / get_sensitivity() * 100.0f;

                return true;
            }

            return false;
        }

        return false;
    }    

    return false;
}

bool get_data_z_axis(float *value)
{
    uint8_t status;
    // check if there is new data available for z-axis
    status_t result = i2c_read(
        I2C_ADDR_LOW, 
        REG_STATUS_REG, 
        1, 
        &status);

    if (result == STATUS_OK)
    {
        // Check the 6th bit of STATUS_REG
        if (status & 0x04)
        {
            // Read the L & H registers (2 bytes)
            status_t result = i2c_read(
                I2C_ADDR_LOW, 
                REG_OUT_Z_L, 
                2, 
                buffer);

            if (result == STATUS_OK)
            {
                // Combine L and H register values
                int16_t axis_value = (int16_t) buffer[1] << 8 | buffer[0];
                // Calculate axis value considering sensitivity
                *value = (float) axis_value / get_sensitivity() * 100.0f;

                return true;
            }

            return false;
        }

        return false;
    }    

    return false;
}

bool set_output_data_rate(
    const uint8_t op_mode, 
    const uint8_t data_rate, 
    const uint8_t fast_odr)
{
    status_t result = i2c_read(
        I2C_ADDR_LOW, 
        REG_CTRL_REG1, 
        1, 
        buffer);

    if (result == STATUS_OK)
    {
        // Create mask for DO[2:0] bits of CTRL_REG1
        uint8_t mask_op_mode = 0b01100000;
        uint8_t mask_data_rate = 0b00011100;
        uint8_t mask_fast_odr = 0b00000010;

        uint8_t value = buffer[0];
        // clear the required bits in buffer
        value &= ~(mask_op_mode | mask_data_rate | mask_fast_odr);
        // Insert new rate bits after shifting to the corresponding places
        value |= ((op_mode << 5) | (data_rate << 2) | (fast_odr << 1));

        status_t result = i2c_write(
            I2C_ADDR_LOW, 
            REG_CTRL_REG1, 
            1, 
            &value); 

        if (result == STATUS_OK)
        {
            return true;
        }

        return false;
    }

    return false;    
}

bool set_interrupt(const bool status)
{
    status_t result = i2c_read(
        I2C_ADDR_LOW, 
        REG_INT_CFG, 
        1, 
        buffer);

    if (result == STATUS_OK)
    {
        uint8_t value;
        if (status == false)
        {
            // Set the LSB to 0 (IEN bit of INT_CFG)
            value = buffer[0] & 0xFE;
        }
        else
        {
            // Set the LSB to 1 (IEN bit of INT_CFG)
            value = buffer[0] | 0x01;
        }

        status_t result = i2c_write(
            I2C_ADDR_LOW, 
            REG_INT_CFG, 
            1, 
            &value);

        if (result == STATUS_OK)
        {
            return true;
        }

        return false;
    }

    return false;
}