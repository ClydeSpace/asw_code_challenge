# AAC-Clydespace Avionics Software Challenge
Submitted by Cris Thomas

## Task Description
Use the C API defined in `i2c.h` to operate on an I2C bus.

Provided example stub implementations of this API in `i2c.c`. Free to
extend the provided stub functions for the API.

Write a device driver that implements the following API:
- Get the full-scale configuration
- Get and set the output data rate
- Enable or disable the interrupt pin
- Read the output data of a specified axis

## Development
The code is implemented in the branch `cris` as instructed.

I have developed a basic driver in C to interface the LIS3MDLTR 3-Axis Magnetometer sensor.

The I2C address for the sensor is 0x1C (SDO/SA1 pin grounded).

## Scope
The driver provides the following functions:

### get_full_scale_config
Reads the full scale configuration bits from CTRL_REG2.
- inputs: (uint8_t) scale
- return: bool

### get_output_data_rate
Reads the output data rate bits from CTRL_REG1.
- inputs: (uint8_t) op_mode, (uint8_t) data_rate, (uint8_t) fast_odr
- return: bool

### get_data_x_axis
Reads the X axis sensor data from OUT_X_L and OUT_X_H.
- inputs: (float) value
- return: bool

### get_data_y_axis
Reads the Y axis sensor data from OUT_Y_L and OUT_Y_H.
- inputs: (float) value
- return: bool

### get_data_z_axis
Reads the Z axis sensor data from OUT_Z_L and OUT_Z_H.
- inputs: (float) value
- return: bool

### float get_sensitivity
Calculates the sensitivity value based on the full scale configuration.
- inputs: none
- return: (float) sensitivity

### set_output_data_rate
Sets the output data rate in CTRL_REG1.
- inputs: (uint8_t) op_mode, (uint8_t) data_rate, (uint8_t) fast_odr
- return: bool

### set_interrupt
Enables or disables the interrupt in INT_CFG.
- inputs: (bool) status
- return: bool

## Extra Thoughts

- What changes you would make to this interfaces for use in an RTOS
environment?

1. Thread safety is very important in an RTOS environment as multiple tasks may access the sensor or the I2C bus.
Hence use mutextes in the critical sections within the I2C communication functions to avoid potential conflicts between concurrent accesses.
2. Enhance error handling (RTOS-specific) to improve the diagnostic capabilities by properly reporting and logging errors.
3. Improve system response by utilizing asynchronous capability, if supported by the RTOS.
4. Ensure compatibility with the RTOS scheduler by adapting the timeouts and delays used in the I2C communication.

- How might the I2C API be improved?

1. Allow configuration of the I2C communication, such as clock frequency, bus timeout, retry, etc.
2. Add proper error handling.
3. Add documentation.
