#ifndef LOGIK_H
#define LOGIK_H
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

typedef struct {
	int co2_Upper;
	int co2_Lower;
	int hum_Lower;
	int hum_Upper;
	int temp_Lower;
	int temp_Upper;
} logik_obj;


void initialise();

void logik_taskCreate();

void saveToFile();

void humChecker(void *pvParameters);

void co2Checker(void *pvParameters);

void tempChecker(void *pvParameters);

#endif  // LOGIK_H