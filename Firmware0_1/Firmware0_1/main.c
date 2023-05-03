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

// Needed for LoRaWAN
#include <lora_driver.h>
#include <status_leds.h>

// Needed for HIH8120 driver initialization
#include <hih8120.h>

// Needed for MHZ19 driver initialization
#include <mh_z19.h>

// define Tasks
void sendData(void *pvParameters);

// define semaphore handle
SemaphoreHandle_t xTestSemaphore;

// Prototype for LoRaWAN handler
void lora_handler_initialise(UBaseType_t lora_handler_task_priority);

// Global variabels
float humidity = 0.0;
float temperature = 0.0;

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
		task1, 
		"Task1" // A name just for humans
		,
		configMINIMAL_STACK_SIZE // This stack size can be checked & adjusted by reading the Stack Highwater
		,
		NULL, 2 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
		,
		NULL);

	xTaskCreate(
		sendData
		,
		"sendData"
		,
		configMINIMAL_STACK_SIZE
		,
		NULL
		,
		3
		,
		NULL);
}

/*-----------------------------------------------------------*/
void sendData(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 300000 / portTICK_PERIOD_MS;

	xLastWakeTime = xTaskGetTickCount();

	for (;;)
	{

		float temperature;
		uint16_t* CO2;
		float humidity;

		// CO2 return code.
		mh_z19_returnCode_t rc;

		if ( HIH8120_OK != hih8120_wakeup() )
		{
       		printf("Could not wake up HIH8120 driver.\n");	
		}

		if ( HIH8120_OK !=  hih8120_measure() )
		{
			printf("Could not measure from HIH8120 driver.\n");
		}

		while (hih8120_isReady())
		{
			sleep(0.06);
		}
		
		printf("Reading Humidity and Temperature.\n");
		temperature = hih8120_getHumidity();
		humidity = hih8120_getTemperature();

		rc = mh_z19_takeMeassuring();
		if (rc != OK)
		{
		    printf("Could not measure ");
		}

		mh_z19_getCo2Ppm(CO2);

		puts("Uploading values %d, %d, %d", temperature, *CO2, humidity)
		lora_driver_payload_t uplink_payload;
		// Setting up amount of data points
		uplink_payload.len = 3;	   // Length of the actual payload
		uplink_payload.portNo = 1; // The LoRaWANport no to sent the message to

		uplink_payload.bytes[0] = temperature;
		uplink_payload.bytes[1] = *CO2;
		uplink_payload.bytes[2] = humidity;

		lora_driver_sendUploadMessage(false, &uplink_payload);
	}
}

/*-----------------------------------------------------------*/

void initialiseDrivers(void *pvParameters)
{

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 500 / portTICK_PERIOD_MS;

	// HIH8120 initialization
	if (HIH8120_OK == hih8120_initialise())
	{
		// Driver initialised OK
		// Always check what hih8120_initialise() returns
	}

	// // MH-Z19 initialization (default USART port is USART3)
	mh_z19_initialise(ser_USART3);
}

/*

TEMPLATE TASK

void task1(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 500 / portTICK_PERIOD_MS; // 500 ms

	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

	for (;;)
	{
		xTaskDelayUntil(&xLastWakeTime, xFrequency);
		puts("Task1"); // stdio functions are not reentrant - Should normally be protected by MUTEX
		PORTA ^= _BV(PA0);
	}
}
*/
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
	// Initialise the LoRaWAN driver without down-link buffer
	lora_driver_initialise(1, NULL);
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
