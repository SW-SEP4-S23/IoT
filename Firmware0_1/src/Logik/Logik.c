
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

	xTaskCreate(humChecker, "HumChecker", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate(co2Checker, "Co2Checker", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
	xTaskCreate(tempChecker, "TempChecker", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
}


void humChecker(void *pvParameters){
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 3000 / portTICK_PERIOD_MS;
	xLastWakeTime = xTaskGetTickCount();
	
	
	
	for(;;){
		xTaskDelayUntil(&xLastWakeTime, xFrequency);
		
		printf("BOBB");
		
		if(xSemaphoreTake(Mutex,pdMS_TO_TICKS(200))==pdTRUE){
			
			if (sensor_getHum() < logikObj.hum_Lower)
			{
				humidifier_Raise();
			}
			else if (sensor_getHum() > logikObj.hum_Upper)
			{
				humidifier_Lower();
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
	const TickType_t xFrequency = 3000 / portTICK_PERIOD_MS;

	xLastWakeTime = xTaskGetTickCount();
	
	for(;;){
		xTaskDelayUntil(&xLastWakeTime, xFrequency);
		if(xSemaphoreTake(Mutex,pdMS_TO_TICKS(200))==pdTRUE){
			
			if (sensor_getCo2() < logikObj.co2_Lower)
			{
				startCo2Generator();
			}
			else if (sensor_getCo2() > logikObj.co2_Upper)
			{
				startVentilation();
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
	const TickType_t xFrequency = 3000 / portTICK_PERIOD_MS;

	xLastWakeTime = xTaskGetTickCount();
	
	for(;;){
		xTaskDelayUntil(&xLastWakeTime, xFrequency);
		
		if(xSemaphoreTake(Mutex,pdMS_TO_TICKS(200))==pdTRUE){
			
			if (sensor_getTemp() < logikObj.temp_Lower)
			{
				ac_Raise();
			}
			else if (sensor_getTemp() > logikObj.temp_Upper)
			{
				ac_Lower();
			}
			xSemaphoreGive(Mutex);
		}
		else
		{
			printf("time out");
		}
	}
}