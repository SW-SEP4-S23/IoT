/*
 * CFile1.c
 *
 * Created: 12/05/2023 10.39.45
 *  Author: jacob
 */

#include "../Headers/Logik.h"
#include "../Headers/7segmentHandler.h"
#include "../Headers/SensorReading.h"
#include <util/delay.h>

float *data;

void raiseLowerFunc()
{
	
	logik_sensor testing;

	sensor_getSensorData(data);

	for (;;)
	{

		_delay_ms(6000);

		if (data[1] < testing.lowTemp)
		{
			return_temp_raised();
		}
		else if (data[1] > testing.maxTemp)
		{
			return_temp_lowered();
		}

		if (data[2] < testing.lowCo2)
		{
			return return_co2_raised();
		}
		else if (data[2] > testing.maxCo2)
		{
			return_co2_lowered();
		}

		if (data[0] < testing.lowHum)
		{
			return_humid_raised();
		}
		else if (data[0] > testing.maxHum)
		{
			return_humid_lowered();
		}
	}
}
