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
#include <semphr.h>
#include <stdio.h>
#include <task.h>
#include <FreeRTOSTraceDriver.h>
#include <ATMEGA_FreeRTOS.h>





SemaphoreHandle_t sharedMutex1  = xSemaphoreCreateMutex();
SemaphoreHandle_t sharedMutex2;
SemaphoreHandle_t sharedMutex3;


logik_obj logikObj;


void initialise(){
			struct logik_obj *pointer;
		// Initialize shared resources
		//sharedMutex1 = xSemaphoreCreateMutex();
		sharedMutex2 = xSemaphoreCreateMutex();
		sharedMutex3 = xSemaphoreCreateMutex();
		float *data;
		sensor_getSensorData(data);
		
/*
		// Create tasks for checking humidity, CO2, and temperature
		xTaskCreate(humChecker, "HumidityChecker", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
		xTaskCreate(co2Checker, "CO2Checker", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
		xTaskCreate(tempChecker, "TemperatureChecker", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

		// Start the scheduler
		vTaskStartScheduler();

		*/
	
}



void humChecker(void *pvParameters){
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 300000 / portTICK_PERIOD_MS;

	xLastWakeTime = xTaskGetTickCount();
	
	for(;;){
			xTaskDelayUntil(&xLastWakeTime, xFrequency);
			
			if(xSemaphoreTake(sharedMutex1,pdMS_TO_TICKS(200))==pdTRUE){
			
					if (data[0] < logikObj.hum_Lower)
					{
						return_humid_raised();
					}
					else if (data[0] > logikObj.hum_Raise)
					{
						return_humid_lowered();
					}
					xSemaphoreGive(sharedMutex1);
					}
			else
			{
				printf("time out");
			}
	}
	
}


void co2Checker(void *pvParameters){
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 300000 / portTICK_PERIOD_MS;

	xLastWakeTime = xTaskGetTickCount();
	
	for(;;){
		xTaskDelayUntil(&xLastWakeTime, xFrequency);
			if(xSemaphoreTake(sharedMutex2,pdMS_TO_TICKS(200))==pdTRUE){
				
				if (data[2] < logikObj.co2_Lower)
				{
					return_co2_raised();
				}
				else if (data[2] > logikObj.co2_Upper)
				{
					return_co2_lowered();
				}
				xSemaphoreGive(sharedMutex2);
			}
			else
			{
				printf("time out");
			}
	}
	
}

void tempChecker(void *pvParameters){
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 300000 / portTICK_PERIOD_MS;

	xLastWakeTime = xTaskGetTickCount();
	
	for(;;){
		xTaskDelayUntil(&xLastWakeTime, xFrequency);
		
			if(xSemaphoreTake(sharedMutex3,pdMS_TO_TICKS(200))==pdTRUE){
				
				if (data[1] < logikObj.temp_Lower)
				{
					return_temp_raised();
				}
				else if (data[1] > logikObj.temp_Raise)
				{
					return_temp_lowered();
				}
				xSemaphoreGive(sharedMutex3);
			}
			else
			{
				printf("time out");
			}
	}
	


