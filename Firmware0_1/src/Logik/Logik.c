#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "../Headers/Logik.h"
#include "../Headers/ModuleHandler.h"
#include "../Headers/SensorReading.h"
#include "../Headers/data_handler.h"

void co2Checker(void *pvParameters);
void tempChecker(void *pvParameters);
void humChecker(void *pvParameters);


void initialise(void){
	xTaskCreate(humChecker, "HumChecker", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate(co2Checker, "Co2Checker", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
	xTaskCreate(tempChecker, "TempChecker", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
}


void humChecker(void *pvParameters){
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 60000 / portTICK_PERIOD_MS;
	xLastWakeTime = xTaskGetTickCount();
	
	
	
	for(;;){
		xTaskDelayUntil(&xLastWakeTime, xFrequency);
			
		if (sensor_getHum() < xData_getHum_lower())
		{
			humidifier_Raise();
		}
		else if (sensor_getHum() > xData_getHum_upper())
		{
			humidifier_Lower();
		}
	}	
}


void co2Checker(void *pvParameters){
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 60000 / portTICK_PERIOD_MS;

	xLastWakeTime = xTaskGetTickCount();
	
	for(;;){
		xTaskDelayUntil(&xLastWakeTime, xFrequency);
			
		if (sensor_getCo2() < xData_getCo2_lower())
		{
			startCo2Generator();
		}
		else if (sensor_getCo2() > xData_getCo2_upper())
		{
			startVentilation();
		}
	}
	
}
	

void tempChecker(void *pvParameters){
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 60000 / portTICK_PERIOD_MS;

	xLastWakeTime = xTaskGetTickCount();
	
	for(;;){
		xTaskDelayUntil(&xLastWakeTime, xFrequency);
			
		if (sensor_getTemp() < xData_getTemp_lower())
		{
			ac_Raise();
		}
		else if (sensor_getTemp() > xData_getTemp_upper())
		{
			ac_Lower();
		}
	}
}