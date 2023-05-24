#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "../Headers/data_handler.h"
#include "../Headers/util.h"

extern SemaphoreHandle_t hum_mutex;
extern SemaphoreHandle_t temp_mutex;
extern SemaphoreHandle_t co2_mutex;
extern SemaphoreHandle_t eep_mutex;

uint8_t read_eep(uint8_t address);
void vSetDefault(void);

void saveLimit(void *pvParameters);

typedef struct {
	uint8_t co2_Upper;
	uint8_t co2_Lower;
	uint8_t hum_Lower;
	uint8_t hum_Upper;
	uint8_t temp_Lower;
	uint8_t temp_Upper;
	uint8_t id;
} limit_t;

limit_t limits;


void vData_handler_initialise() {
    uint8_t id = eeprom_read_byte((uint8_t*)0);

    if (id == 255)
    {
        vSetDefault();
        return;
    }

    limits.id = id;
    limits.co2_Upper = eeprom_read_byte((uint8_t*)1);
    limits.co2_Lower = eeprom_read_byte((uint8_t*)2);
    limits.hum_Upper = eeprom_read_byte((uint8_t*)3);
    limits.hum_Lower = eeprom_read_byte((uint8_t*)4);
    limits.temp_Upper = eeprom_read_byte((uint8_t*)5);
    limits.temp_Lower = eeprom_read_byte((uint8_t*)6);
}

void vSetDefault(void) {
    vData_setHum_lower(0);
    vData_setHum_upper(95);

    vData_setTemp_lower(-40);
    vData_setTemp_upper(50);

    vData_setCo2_lower(0);
    vData_setCo2_lower(5);
}


uint8_t read_eep(uint8_t address) {
	
	uint8_t valueRead = eeprom_read_byte((uint8_t*)address);

    return valueRead;
}


void write_eep(uint8_t address, uint8_t input) {
    eeprom_write_byte((uint8_t*)address, input);
}


void saveLimit(void *pvParameters) {
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 21600000 / portTICK_PERIOD_MS;

    xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        xTaskDelayUntil(&xLastWakeTime, xFrequency);

        protected_printf("Saving values");
        if (xSemaphoreTake(eep_mutex,pdMS_TO_TICKS(200))==pdTRUE) {
			
			eeprom_write_byte((uint8_t*)0, xData_getId());
			eeprom_write_byte((uint8_t*)1, xData_getCo2_upper());
			eeprom_write_byte((uint8_t*)2, xData_getCo2_lower());
			eeprom_write_byte((uint8_t*)3, xData_getHum_upper());
			eeprom_write_byte((uint8_t*)4, xData_getHum_lower());
			eeprom_write_byte((uint8_t*)5, xData_getTemp_upper());
			eeprom_write_byte((uint8_t*)6, xData_getTemp_lower());
            xSemaphoreGive(eep_mutex);
        }
    }
}


uint8_t xData_getCo2_upper(void) {
    return limits.co2_Upper;
}

uint8_t xData_getCo2_lower(void) {
    return limits.co2_Lower;
}

uint8_t xData_getHum_upper(void) {
    return limits.hum_Upper;
}

uint8_t xData_getHum_lower(void) {
    return limits.hum_Lower;
}

uint8_t xData_getTemp_upper(void) {
    return limits.temp_Upper;
}

uint8_t xData_getTemp_lower(void) {
    return limits.temp_Lower;
	
}

uint8_t xData_getId() {
    return limits.id;
}

void vData_setCo2_upper(uint8_t value) {
    if (xSemaphoreTake(co2_mutex,pdMS_TO_TICKS(200))==pdTRUE) {
        limits.co2_Upper = value;

        xSemaphoreGive(co2_mutex);
    }
}

void vData_setCo2_lower(uint8_t value) {
    if (xSemaphoreTake(co2_mutex,pdMS_TO_TICKS(200))==pdTRUE) {
        limits.co2_Lower = value;

        xSemaphoreGive(co2_mutex);
    }
}

void vData_setHum_upper(uint8_t value) {
    if (xSemaphoreTake(hum_mutex,pdMS_TO_TICKS(200))==pdTRUE) {
        limits.hum_Upper = value;

        xSemaphoreGive(hum_mutex);
    }
}

void vData_setHum_lower(uint8_t value) {
    if (xSemaphoreTake(hum_mutex,pdMS_TO_TICKS(200))==pdTRUE) {
        limits.hum_Lower = value;

        xSemaphoreGive(hum_mutex);
    }
}

void vData_setTemp_upper(uint8_t value) {
   if (xSemaphoreTake(temp_mutex,pdMS_TO_TICKS(200))==pdTRUE) {
        limits.temp_Upper = value;

        xSemaphoreGive(temp_mutex);
    }
}

void vData_setTemp_lower(uint8_t value) {
    if (xSemaphoreTake(temp_mutex,pdMS_TO_TICKS(200))==pdTRUE) {
        limits.temp_Lower = value;

        xSemaphoreGive(temp_mutex);
    }
}

void vData_setId(uint8_t value) {
    limits.id = value;
}

void data_vTaskCreate(void)
{
    xTaskCreate(saveLimit, "SaveLimit", configMINIMAL_STACK_SIZE, NULL, 99, NULL);
}