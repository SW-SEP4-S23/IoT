#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "../Headers/Logik.h"
#include "../Headers/SensorReading.h"
#include "../Headers/DownLinkValidator.h"
#include "../Headers/CommHandler.h"
#include <lora_driver.h>
#include "../Headers/data_handler.h"

SemaphoreHandle_t Mutex;

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
        uplink_payload.bytes[3] = xData_getId();

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

			xData_setCo2_lower(downlinkPayload.bytes[2]);
			xData_setCo2_upper(downlinkPayload.bytes[3]);
			xData_setHum_lower(downlinkPayload.bytes[5]);
			xData_setHum_upper(downlinkPayload.bytes[4]);
			xData_setTemp_lower(downlinkPayload.bytes[0]);
			xData_setTemp_upper(downlinkPayload.bytes[1]);
			xData_setId(downlinkPayload.bytes[6]);
		}
	}
}

void comm_vTaskCreate(void)
{
    xTaskCreate(sendData, "SendData", configMINIMAL_STACK_SIZE, NULL, 4, NULL);
    xTaskCreate(recieveData, "RecieveData", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
}