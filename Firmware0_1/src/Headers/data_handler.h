#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <EEPROM.h>
#include <stdint.h>

void vData_handler_initialise();

void data_vTaskCreate();

uint8_t xData_getCo2_upper();
uint8_t xData_getCo2_lower();
uint8_t xData_getHum_lower();
uint8_t xData_getHum_upper();
uint8_t xData_getTemp_lower();
uint8_t xData_getTemp_upper();

uint8_t xData_getCo2_upper();
uint8_t xData_getCo2_lower();
uint8_t xData_getHum_upper();
uint8_t xData_getHum_lower();
uint8_t xData_getTemp_upper();
uint8_t xData_getTemp_lower();

#endif  // DATA_HANDLER_H