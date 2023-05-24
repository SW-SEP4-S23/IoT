#ifndef COMM_HANDLER_H
#define COMM_HANDLER_H

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

void comm_vTaskCreate(void);

#endif  // COMM_HANDLER_H