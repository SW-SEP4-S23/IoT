#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <stdint.h>
#include "../EEPROM/src/EEPROM.h"


void vData_handler_initialise();

void data_vTaskCreate();

uint8_t xData_getCo2_upper();
uint8_t xData_getCo2_lower();
uint8_t xData_getHum_lower();
uint8_t xData_getHum_upper();
uint8_t xData_getTemp_lower();
uint8_t xData_getTemp_upper();
uint8_t xData_getId();

void xData_setCo2_upper(uint8_t value);
void xData_setCo2_lower(uint8_t value);
void xData_setHum_upper(uint8_t value);
void xData_setHum_lower(uint8_t value);
void xData_setTemp_upper(uint8_t value);
void xData_setTemp_lower(uint8_t value);
void xData_setId(uint8_t value);

#endif  // DATA_HANDLER_H