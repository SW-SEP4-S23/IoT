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

#endif  // LOGIK_H