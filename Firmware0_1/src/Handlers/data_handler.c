#include "../Headers/data_handler.h"

extern SemaphoreHandle_t hum_mutex;
extern SemaphoreHandle_t temp_mutex;
extern SemaphoreHandle_t co2_mutex;
extern SemaphoreHandle_t eep_mutex;

typedef struct {
	uint8_t co2_Upper;
	uint8_t co2_Lower;
	uint8_t hum_Lower;
	uint8_t hum_Upper;
	uint8_t temp_Lower;
	uint8_t temp_Upper;
	uint8_t id;
} limit_t;


void vData_handler_initialise() {
    uint8_t id = read_eep(0);

    if (id == 255)
    {
        vSetDefault();
        return;
    }

    limit_t.id = id;
    limit_t.co2_upper = read_eep(1);
    limit_t.co2_lower = read_eep(2);
    limit_t.hum_upper = read_eep(3);
    limit_t.hum_lower = read_eep(4);
    limit_t.temp_upper = read_eep(5);
    limit_t.temp_lower = read_eep(6);
}

void vSetDefault(void) {
    vData_setHum_lower(0);
    vData_setHum_upper(95);

    vData_setTemp_lower(-40);
    vData_setTemp_upper(50);

    vData_setCo2_lower(0);
    vData_setCo2_lower(5000);
}


uint8_t read_eep(uint8_t address) {
    int valueRead = EEPROM.read(address);

    return (uint8_t) valueRead;
}


uint8_t write_eep(uint8_t address, int input) {
    EEPROM.write(address, input);
}


void saveLimit(void *pvParameters) {
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 21600000 / portTICK_PERIOD_MS;

    xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        xTaskDelayUntil(&xLastWakeTime, xFrequency);

        puts("Saving values");
        xSemaphoreTake(eep_mutex,pdMS_TO_TICKS(200))==pdTRUE {
            EEPROM.write(0, xData_getId());
            EEPROM.write(1, xData_getCo2_upper());
            EEPROM.write(2, xData_getCo2_lower());
            EEPROM.write(3, xData_getHum_upper());
            EEPROM.write(4, xData_getHum_lower());
            EEPROM.write(5, xData_getTemp_upper());
            EEPROM.write(6, xData_getTemp_lower());
            xSemaphoreGive(eep_mutex);
        }
    }
}


uint8_t xData_getCo2_upper(void) {
    return limit_t.co2_upper;
}

uint8_t xData_getCo2_lower(void) {
    return limit_t.co2_lower;
}

uint8_t xData_getHum_upper(void) {
    return limit_t.hum_upper;
}

uint8_t xData_getHum_lower(void) {
    return limit_t.hum_lower;
}

uint8_t xData_getTemp_upper(void) {
    return limit_t.temp_upper;
}

uint8_t xData_getTemp_lower(void) {
    return limit_t.temp_lower;
}

uint8_t xData_getId() {
    return limit_t.id;
}

void vData_setCo2_upper(uint8_t value) {
    xSemaphoreTake(co2_mutex,pdMS_TO_TICKS(200))==pdTRUE {
        limit_t.co2_upper = value;

        xSemaphoreGive(co2_mutex);
    }
}

void vData_setCo2_lower(uint8_t value) {
    xSemaphoreTake(co2_mutex,pdMS_TO_TICKS(200))==pdTRUE {
        limit_t.co2_lower = value;

        xSemaphoreGive(co2_mutex);
    }
}

void vData_setHum_upper(uint8_t value) {
    xSemaphoreTake(hum_mutex,pdMS_TO_TICKS(200))==pdTRUE {
        limit_t.hum_upper = value;

        xSemaphoreGive(hum_mutex);
    }
}

void vData_setHum_lower(uint8_t value) {
    xSemaphoreTake(hum_mutex,pdMS_TO_TICKS(200))==pdTRUE {
        limit_t.hum_lower = value;

        xSemaphoreGive(hum_mutex);
    }
}

void vData_setTemp_upper(uint8_t value) {
    xSemaphoreTake(temp_mutex,pdMS_TO_TICKS(200))==pdTRUE {
        limit_t.temp_upper = value;

        xSemaphoreGive(temp_mutex);
    }
}

void vData_setTemp_lower(uint8_t value) {
    xSemaphoreTake(temp_mutex,pdMS_TO_TICKS(200))==pdTRUE {
        limit_t.temp_lower = value;

        xSemaphoreGive(temp_mutex);
    }
}

void data_vTaskCreate(void)
{
    xTaskCreate(saveLimit, "SaveLimit", configMINIMAL_STACK_SIZE, NULL, 99, NULL);
}