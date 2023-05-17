/*
 * CFile1.c
 *
 * Created: 12/05/2023 10.39.45
 *  Author: jacob
 */

#include "Headers/Logik.h"
#include "Headers/7segmentHandler.h"
#include "Headers/SensorReading.h"
#include <util/delay.h>

float data *;

void raiseLowerFunc()
{

	sensor_getSensorData(data);

	for (;;)
	{

		_delay_ms(6000);

		if (data[1] < logik_sensor.lowTemp)
		{
			return_temp_raised();
		}
		if
			else(data[1] > logik_sensor.maxTemp)
			{
				return_temp_lowered();
			}

		if (data[2] < logik_sensor.lowCo2)
		{
			return return_co2_raised();
		}
		if
			else(data[2] > logik_sensor.maxCo2)
			{
				return_co2_lowered();
			}

		if (data[0] < logik_sensor.lowHum)
		{
			return_humid_raised();
		}
		if
			else(data[0] > logik_sensor.maxHum)
			{
				return_humid_lowered();
			}
	}
}