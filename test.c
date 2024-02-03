#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "driver.h"

// All values in the test are initialized to random invalid values to detect errors
int main() {
    srand(time(NULL));
    // repeat the test so both succesful and failed reads are simulated
    for (int i = 0; i < 100; i++) {
        int16_t data = 5;
        if (get_axis_data(X, &data) == STATUS_OK) {
            // Expect data to be cleared and set correctly
            assert((data & 0xffff) == 0xfeff);
            printf("Read expected data\n");
        } else {
            // Expect data to be unchanged
            assert(data == 5);
            printf("Data wasn't corrupted on bad read\n");
        }
        data_rate_conf dr_conf = 1;
        if (get_data_rate(&dr_conf) == STATUS_OK) {
            // Expect dr_conf to be cleared and set correctly
            assert(dr_conf == _1K);
            printf("Read expected data rate config\n");
        } else {
            assert(dr_conf == 1);
            printf("Data wasn't corrupted on bad read\n");
        }
        full_scale_conf fs_conf = 10;
        if (get_full_scale_conf(&fs_conf) == STATUS_OK) {
            assert(fs_conf == _16Gs);
            printf("Read expected full scale config\n");
        } else {
            assert(fs_conf == 10);
            printf("Data wasn't corrupted on bad read\n");
        }
        // Output of setters can be inspected manually from the console
        puts("Setting data rate to 0.625Hz");
        set_data_rate(_0P625);
        puts("Setting data rate to 80Hz");
        set_data_rate(_80);
        puts("Setting data rate to 1KHz");
        set_data_rate(_1K);
        puts("Disabling interrupt pin");
        set_interrupt(false);
        puts("Enabling interrupt pin");
        set_interrupt(true);
    }
    return 0;
}