
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "../Headers/Logik.h"
#include "../Headers/7segmentHandler.h"
#include "../Headers/SensorReading.h"

SemaphoreHandle_t Mutex;

logik_obj logikObj;

void initialise(void){
	if (Mutex == NULL) // Check to confirm that the Semaphore has not already been created.
	{
		Mutex = xSemaphoreCreateMutex(); // Create a mutex semaphore.
		if ((Mutex) != NULL)
		{
			xSemaphoreGive((Mutex)); // Make the mutex available for use, by initially "Giving" the Semaphore.
		}
	}
}


void humChecker(void *pvParameters){
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 300000 / portTICK_PERIOD_MS;
	xLastWakeTime = xTaskGetTickCount();
	
	
	for(;;){
		xTaskDelayUntil(&xLastWakeTime, xFrequency);
		
		if(xSemaphoreTake(Mutex,pdMS_TO_TICKS(200))==pdTRUE){
			
			if (data[0] < logikObj.hum_Lower)
			{
				return_humid_raised();
			}
			else if (data[0] > logikObj.hum_Raise)
			{
				return_humid_lowered();
			}
			xSemaphoreGive(Mutex);
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
		if(xSemaphoreTake(Mutex,pdMS_TO_TICKS(200))==pdTRUE){
			
			if (data[2] < logikObj.co2_Lower)
			{
				return_co2_raised();
			}
			else if (data[2] > logikObj.co2_Upper)
			{
				return_co2_lowered();
			}
			xSemaphoreGive(Mutex);
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
		
		if(xSemaphoreTake(Mutex,pdMS_TO_TICKS(200))==pdTRUE){
			
			if (data[1] < logikObj.temp_Lower)
			{
				return_temp_raised();
			}
			else if (data[1] > logikObj.temp_Raise)
			{
				return_temp_lowered();
			}
			xSemaphoreGive(Mutex);
		}
		else
		{
			printf("time out");
		}
	}
}