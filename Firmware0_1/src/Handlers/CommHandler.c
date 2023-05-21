#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "../Headers/Logik.h"

SemaphoreHandle_t Mutex;

extern logikObj;


void sendData(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 300000 / portTICK_PERIOD_MS;

	xLastWakeTime = xTaskGetTickCount();

	for (;;)
	{
		xTaskDelayUntil(&xLastWakeTime, xFrequency);

		puts("Uploading values");
		lora_driver_payload_t uplink_payload;
		// Setting up amount of data points
		uplink_payload.len = 3;	   // Length of the actual payload
		uplink_payload.portNo = 1; // The LoRaWANport no to sent the message to

		// Hvis man placerede float Data[3] uden for for-loopet, ville man s� ikke kunne undg� at bruge memory management p� det?

		// Reading the sensor data

		sensor_getSensorData(data);

		// Saving sensor data to the uplink
		uplink_payload.bytes[0] = data[0];
		uplink_payload.bytes[1] = data[1];
		uplink_payload.bytes[2] = data[2];
		uplink_payload.bytes[3] = 1;

		// Sending uplink message.
		lora_driver_sendUploadMessage(false, &uplink_payload);
	}
}

/*-----------------------------------------------------------*/
void recieveData(void *pvParameters)
{

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 300000 / portTICK_PERIOD_MS;

	xLastWakeTime = xTaskGetTickCount();

	lora_driver_resetRn2483(1);
	vTaskDelay(2);
	lora_driver_resetRn2483(0);
	vTaskDelay(150);
	lora_driver_flushBuffers();

	for (;;)
	{

		xTaskDelayUntil(&xLastWakeTime, xFrequency);

		lora_driver_payload_t downlinkPayload;

		xMessageBufferReceive(downLinkMessageBufferHandle, &downlinkPayload, sizeof(lora_driver_payload_t), portMAX_DELAY);
		printf("DOWN LINK: from port: %d with %d bytes received!", downlinkPayload.portNo, downlinkPayload.len); // Just for Debug
		if (2 < downlinkPayload.len)
		{ // Check that we have got the expected 7 bytes
			// decode the payload into our variales
			logik.temp_Lower = downlinkPayload.bytes[0];
			logik.temp_Raise = downlinkPayload.bytes[1];
			logik.co2_Lower = downlinkPayload.bytes[2];
			logik.co2_Upper = downlinkPayload.bytes[3];
			logik.hum_Lower = downlinkPayload.bytes[4];
			logik.hum_Raise = downlinkPayload.bytes[5];
			logik.id = (downlinkPayload.bytes[6]);

			// printf for test
			printf("The current max temp setting is: %d and uniq id is: %d", logik.temp_Lower, logik.id);
		}
	}
}