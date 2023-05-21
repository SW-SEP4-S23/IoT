/*
 * main.c
 * Author : IHA
 *
 * Example main file including LoRaWAN setup
 * Just for inspiration :)
 */

#include <stdio.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <FreeRTOSTraceDriver.h>
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <stdio_driver.h>
#include <serial.h>
#include <time.h>
#include <util/delay.h>
#include "Headers/Logik.h"
#include "Headers/7segmentHandler.h"

// Needed for LoRaWAN
#include <lora_driver.h>
#include <status_leds.h>
// Needed for HIH8120 driver initialization
#include <hih8120.h>

// Needed for MHZ19 driver initialization
#include <mh_z19.h>

// Needed for 7-segment driver initialization
#include <display_7seg.h>

#include "Headers/SensorReading.h"

// define Tasks
void sendData(void *pvParameters);
void recieveData(void *pvParameters);
void humChecker(void *pvParameters);
void co2Checker(void *pvParameters);
void tempChecker(void *pvParameters);



logik_obj logik;


float *data;

// define semaphore handle
SemaphoreHandle_t xTestSemaphore;

MessageBufferHandle_t downLinkMessageBufferHandle;

// Prototype for LoRaWAN handler
void lora_handler_initialise(UBaseType_t lora_handler_task_priority);

/*-----------------------------------------------------------*/
void create_tasks_and_semaphores(void)
{
	// Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
	// because it is sharing a resource, such as the Serial port.
	// Semaphores should only be used whilst the scheduler is running, but we can set it up here.
	if (xTestSemaphore == NULL) // Check to confirm that the Semaphore has not already been created.
	{
		xTestSemaphore = xSemaphoreCreateMutex(); // Create a mutex semaphore.
		if ((xTestSemaphore) != NULL)
		{
			xSemaphoreGive((xTestSemaphore)); // Make the mutex available for use, by initially "Giving" the Semaphore.
		}
	}

	xTaskCreate(
		sendData,
		"sendData",
		configMINIMAL_STACK_SIZE,
		NULL,
		3,
		NULL);

	xTaskCreate(
		recieveData,
		"recieveData",
		configMINIMAL_STACK_SIZE,
		NULL,
		4,
		NULL);
		
	xTaskCreate(
		humChecker,
		"humChecker",
		configMINIMAL_STACK_SIZE,
		NULL,
		5,
		NULL);
		
			
	xTaskCreate(
		co2Checker,
		"co2Checker",
		configMINIMAL_STACK_SIZE,
		NULL,
		6,
		NULL);
				
	xTaskCreate(
		tempChecker,
		"tempChecker",
		configMINIMAL_STACK_SIZE,
		NULL,
		7,
		NULL);
}

/*-----------------------------------------------------------*/

void initialiseDrivers()
{

	// HIH8120 initialization
	if (HIH8120_OK == hih8120_initialise())
	{
		// Driver initialised OK
		// Always check what hih8120_initialise() returns
	}

	// // MH-Z19 initialization (default USART port is USART3)
	mh_z19_initialise(ser_USART3);
}

/*-----------------------------------------------------------*/

void initialiseSystem()
{
	// Set output ports for leds used in the example
	DDRA |= _BV(DDA0) | _BV(DDA7);

	// initialise trace driver
	trace_init();

	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdio_initialise(ser_USART0);
	// Let's create some tasks

	// initialiseDrivers - handles driver initailazation for modules HIH8120, MH-Z19
	initialiseDrivers();

	create_tasks_and_semaphores();

	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialisation vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Status Leds driver
	status_leds_initialise(5); // Priority 5 for internal task
	// Initialise the LoRaWAN driver with down-link buffer
	downLinkMessageBufferHandle = xMessageBufferCreate(sizeof(lora_driver_payload_t) * 2); // Here I make room for two downlink messages in the message buffer
	lora_driver_initialise(ser_USART1, downLinkMessageBufferHandle);					   // The parameter is the USART port the RN2483 module is connected to - in this case USART1 - here no message buffer for down-link messages are defined
	// Create LoRaWAN task and start it up with priority 3
	lora_handler_initialise(3);
}

/*-----------------------------------------------------------*/
int main(void)
{
	initialiseSystem(); // Must be done as the very first thing!!
	printf("Program started!!\n");
	vTaskStartScheduler(); // Initialise and run the freeRTOS scheduler. Execution should never return from here.

	/* Replace with your application code */
	while (1)
	{
	}
}
