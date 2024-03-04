#include <stdio.h>
#include "lis3mdl.h"
#include "comm_interface.h"
#include "i2c.h"

// Initialize communication interface
comm_interface_t comm = {
    .init = i2c_init, // Assuming i2c_init function is provided
    .read = i2c_read,
    .write = i2c_write,        
    .config.dev_addr = LIS3MDL_ADDRESS,
};


int main() {

    // Initialize LIS3MDL driver with the communication interface
    lis3mdl_init_comm_interface(&comm);

    // Read magnetic data for the x-axis
    int16_t magnetic_data;
    lis3mdl_err_t result = lis3mdl_read_axis_data('x', &magnetic_data);

    // Check if reading was successful
    if (result == LIS3MDL_OK) {
        printf("Magnetic data for the x-axis: %d\n", magnetic_data);
    } else {
        printf("Failed to read magnetic data for the x-axis.\n");
    }

    return 0;
}
