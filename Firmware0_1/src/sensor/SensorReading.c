#include <stdio.h>
#include <stdio_driver.h>
#include <ATMEGA_FreeRTOS.h>
#include <mh_z19.h>
#include <hih8120.h>
#include "../Headers/util.h"
#include "../Headers/SensorReading.h"

void co2_callBack(void);

uint16_t ppm;

void sensor_initialise(void) {
	mh_z19_initialise(ser_USART3);
	hih8120_driverReturnCode_t rc = hih8120_initialise();
	protected_printf("HIH8120 initialisation >%s<\n", hih8120_xReturnCodes_to_text(rc));
}

void sensor_wakeUp(void)
{
    hih8120_driverReturnCode_t wake_rc = hih8120_wakeup();

	protected_printf("HIH8120 Wake up: >%s<\n", hih8120_xReturnCodes_to_text(wake_rc));
}

uint8_t sensor_getTemp(void) {
	sensor_wakeUp();

	vTaskDelay(pdMS_TO_TICKS(60UL));

	hih8120_measure();

	 while (!(hih8120_isReady())) {

		vTaskDelay(pdMS_TO_TICKS(60UL));
	 }
	 return( hih8120_getTemperature());
}


uint8_t sensor_getHum(void) {

	sensor_wakeUp();

	vTaskDelay(pdMS_TO_TICKS(60UL));

	hih8120_measure();

	while (!(hih8120_isReady())) {

		vTaskDelay(pdMS_TO_TICKS(60UL));
	}
	return( hih8120_getHumidity());
}

uint16_t sensor_getCo2(void){
	
	mh_z19_returnCode_t co2_rc = mh_z19_takeMeassuring();

	vTaskDelay(pdMS_TO_TICKS(100UL));

	protected_printf("MH_Z19 Measure status: >%s<\n", mh_z19_xReturnCodes_to_text(co2_rc));

	mh_z19_getCo2Ppm(&ppm);
	  
	return ppm;
}

