#ifndef UTIL_H
#define UTIL_H

#include <hih8120.h>
#include <mh_z19.h>
#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>
#include <task.h>

char* mh_z19_xReturnCodes_to_text(mh_z19_returnCode_t returnMessage);
char* hih8120_xReturnCodes_to_text(hih8120_driverReturnCode_t returnMessage);

extern SemaphoreHandle_t printMutex;

void protected_printf(const char* _fmt, ...);

#endif  // UTIL_H