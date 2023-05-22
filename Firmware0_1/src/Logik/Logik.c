
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "../Headers/Logik.h"
#include "../Headers/ModuleHandler.h"
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
			
			if (sensor_getHum() < logikObj.hum_Lower)
			{
				return_humid_raised();
			}
			else if (sensor_getHum() > logikObj.hum_Raise)
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
			
			if (sensor_getCo2() < logikObj.co2_Lower)
			{
				return_co2_raised();
			}
			else if (sensor_getCo2() > logikObj.co2_Upper)
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
			
			if (sensor_getTemp() < logikObj.temp_Lower)
			{
				return_temp_raised();
			}
			else if (sensor_getTemp() > logikObj.temp_Raise)
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