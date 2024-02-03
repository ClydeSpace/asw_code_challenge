# AAC-Clydespace Avionics Software Challenge

# Submitted by Mani Yazdankhah

## Desing notes:  
My goal was to create an easy-to-use and easy-to-extend driver API for LIS3MDL. To achieve this, I used enums to represent possible values for each configuration and wrote reusable register setter and getter functions.  
This API is not optimized for performance as it does extra computation (calculating trailing zeros for masks at runtime). It is possible to add a `ctz` property to the `field` struct and use an init method to do this pre-computation, but I skipped that step due to time constraints. Additionally, reading the data from one of the axes uses an intermediate buffer, but it would be more efficient to call the `read_byte` method twice and use its value directly. Depending on the hardware architecture, it might be more memory-efficient to return the values directly from the API instead of passing pointers everywhere and using the return for error checking. C++ provides the `std::optional` but C doesn't have an equivalent type. Defining my own option type would be possible, but again, I skipped that due to time constraints.

For this implementation, setting values in a register requires first reading the existing data in that register, which adds additional overhead. It might be a good idea to create a struct that stores the current configuration of all registers, which acts as a cache and represents the state of the registers on the device. Therefore, we can just perform masking, clearing, and setting the desired bits without needing to read from the device.

These optimizations might not be required if the target MCU is fast enough or has a large enough RAM.  

# Implemented methods:

## set_bus_address :  
Sets the I2C bus address, which is used for all communications.

## get_full_scale_conf:  
Reads the full scale configuration from `CTRL_REG2` and stores it in a `full_scale_conf` enum.

## get_data_rate:  
Reads the data rate stored at `CTRL_REG1` based on `OM`, 'DO', and `FAST_ODR` fields in datasheet table 18.

## set_data_rate:  
Writes the desired data rate to `CTRL_REG1`. If the selected data rate is higher than 80 Hz, `FAST_ODR` is set and `OM` is changed to achieve the desired data rate.

## set_interrupt:  
Enables or disables the interrupt pin

## get_axis_data:  
Reads 2 bytes from the registers corresponding to the selected axis.  

## Scope  
I spent about 4 hours on this task. Given more time, I think I could write a cleaner and more performant driver, but the current version should be easily extensible to provide APIs for every register in the datasheet.

## Compilation and testing:  
A makefile is included (GCC 13 is used to compile and verify). Running `make` followed by `./test` should run the test.

To test the driver, I added randomness to the I2C implementation to simulate communication failures (adjustable value, but default is an 80% chance of success). I then loop for 100 iterations and check that on successful reads I received the expected value, and on failed reads I didn't corrupt any data. To test setters, I just manually inspected the console output.  

## Extra Thoughts

- How might the I2C API be improved? The I2C driver is not very user-friendly as the original implementation only provides array communication, but most registers in embedded applications are just one byte (the number of different configurations for any given behavior is extremely unlikely to exceed 255 possibilities). Also, using buffers uses more memory and is slower since the results might need to be manipulated. In my opinion, the Arduino I2C API is much better because you can begin a transmission, send and receive the data you need one byte at a time, and then end the transmission (https://docs.arduino.cc/learn/communication/wire/#wire-library).
