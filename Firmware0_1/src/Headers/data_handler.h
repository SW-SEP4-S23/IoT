#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <avr/eeprom.h>
#include <stdint.h>


void vData_handler_initialise();

void data_vTaskCreate(void);

uint8_t xData_getCo2_upper();
uint8_t xData_getCo2_lower();
uint8_t xData_getHum_lower();
uint8_t xData_getHum_upper();
uint8_t xData_getTemp_lower();
uint8_t xData_getTemp_upper();
uint8_t xData_getId();

void vData_setCo2_upper(uint8_t value);
void vData_setCo2_lower(uint8_t value);
void vData_setHum_upper(uint8_t value);
void vData_setHum_lower(uint8_t value);
void vData_setTemp_upper(uint8_t value);
void vData_setTemp_lower(uint8_t value);
void vData_setId(uint8_t value);

#endif  // DATA_HANDLER_H