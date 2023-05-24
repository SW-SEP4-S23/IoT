#include <stdbool.h>
#include "../Headers/DownLinkValidator.h"
#include "../Headers/data_handler.h"

int validateDownlink(float hum_lower, float hum_upper, float co2_lower, float co2_upper, float temp_lower, float temp_upper, float id) {

    // Checks the current ID against the saved ID.

    if (id == xData_getId) {
        return 0;
    }

    // Checks Humidity. Settings cannot go below 0, as that's impossible and 
    // not above 95 as the CO2 driver would not function in that envoirment.

    if (hum_lower < 0 || hum_upper > 95) {
        return 0;
    }

    // The upper and lower limits here are dictated by what the sensor can read.

    if (co2_lower < 0 || co2_upper > 5000) {
        return 0;
    }

    // The lower limit is the lowest the sensor can read.
    // The upper limit is the max temperature the CO2 sensor can work in.

    if (temp_lower < -40 || temp_upper > 50) {
        return 0;
    }

    return 1;
}