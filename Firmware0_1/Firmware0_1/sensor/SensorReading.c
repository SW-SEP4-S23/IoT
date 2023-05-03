#include <stdio.h>
#include <stdio_driver.h>
#include <util/delay.h>
#include <mh_z19.h>

#include "SensorReading.h"


float sensor_wakeUp() {
    hih8120_driverReturnCode_t wake_rc = hih8120_wakeup();

    printf("HIH8120 Wake up status: %c\n", wake_rc);
}

void *sensor_getSensorData(*dataCollection) {

    sensor_wakeUp();

    _delay_ms(60);

    hih8120DriverReturnCode_t measure_rc = hih8120_measure();

    printf("HIH8120 Measure status: %c\n", measure_rc);

    while (hih8120_isReady())
    {
        _delay_ms(60);
    }
    

	dataCollection[0] = hih8120_getHumidity();
	dataCollection[1] = hih8120_getTemperature();

    uint16_t ppm;
    mh_z19_returnCode_t co2_rc = mh_z19_takeMeassuring();
    printf("MH_Z19 Measure status: %c", co2_rc);
    
    mh_z19_getCo2Ppm(*ppm);

    dataCollection[2] = (float)ppm;
}