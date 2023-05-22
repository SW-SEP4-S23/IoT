#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "../Headers/Logik.h"
#include "../Headers/SensorReading.h"
#include "../Headers/DownLinkValidator.h"
#include "../Headers/CommHandler.h"
#include <lora_driver.h>

SemaphoreHandle_t Mutex;

extern float currentID;
extern logik_obj logikObj;
extern MessageBufferHandle_t downLinkMessageBufferHandle;

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
        uplink_payload.len = 4;       // Length of the actual payload
        uplink_payload.portNo = 1;    // The LoRaWAN port number to send the message to

        // Saving sensor data to the uplink
        uplink_payload.bytes[0] = sensor_getTemp();
        uplink_payload.bytes[1] = sensor_getCo2();
        uplink_payload.bytes[2] = sensor_getHum();
        uplink_payload.bytes[3] = currentID;

        // Sending uplink message
        lora_driver_sendUploadMessage(false, &uplink_payload);
    }
}


void recieveData(void *pvParameters)
{

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 150000 / portTICK_PERIOD_MS;

	xLastWakeTime = xTaskGetTickCount();

	for (;;)
	{

		xTaskDelayUntil(&xLastWakeTime, xFrequency);

		lora_driver_payload_t downlinkPayload;

		xMessageBufferReceive(downLinkMessageBufferHandle, &downlinkPayload, sizeof(lora_driver_payload_t), portMAX_DELAY);
		if (6 < downlinkPayload.len)
		{

			// Checks if all the numbers are validated

			if (validateDownlink(downlinkPayload.bytes[4], downlinkPayload.bytes[5], downlinkPayload.bytes[2], downlinkPayload.bytes[3], downlinkPayload.bytes[0], downlinkPayload.bytes[1], downlinkPayload.bytes[6]) == 0)
			{
				// if not, returns 1
				return 1;
			}

			// Sets the values to the logikObj

			logikObj.co2_Lower = downlinkPayload.bytes[2];
			logikObj.co2_Upper = downlinkPayload.bytes[3];
			logikObj.hum_Upper = downlinkPayload.bytes[4];
			logikObj.hum_Lower = downlinkPayload.bytes[5];
			logikObj.temp_Lower = downlinkPayload.bytes[0];
			logikObj.temp_Upper = downlinkPayload.bytes[1];
			currentID = downlinkPayload.bytes[6];
		}
	}
}

void comm_vTaskCreate(void)
{
    xTaskCreate(sendData, "SendData", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
    xTaskCreate(recieveData, "RecieveData", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
}