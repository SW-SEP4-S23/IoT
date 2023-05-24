#include <stdio.h>
#include <stdio.h>
#include <stdarg.h>	//Used for varadic function
#include <ATMEGA_FreeRTOS.h>
#include "../Headers/util.h"

const char* mh_z19_returnCode_t_text[] = {
    "MHZ19_OK", 	/**< Everything went well */
	"MHZ19_NO_MEASSURING_AVAILABLE", /**< No results are received from the sensor */
	"MHZ19_NO_SERIAL", /**< No serial driver is specified */
	"MHZ19_PPM_MUST_BE_GT_999"
};

const char* hih8120_driverReturnCodes_text = {
	"HIH8120_OK",	/**< Everything went well */
	"HIH8120_OUT_OF_HEAP", /**< Not enough heap to initialise the driver */
	"HIH8120_DRIVER_NOT_INITIALISED", /**< Driver must be initialise before use */
	"HIH8120_TWI_BUSY" /**< The two wire/I2C interface is busy */
};

char* hih8120_xReturnCodes_to_text(hih8120_driverReturnCode_t returnMessage) {
    return hih8120_driverReturnCodes_text[returnMessage];
}

char* mh_z19_xReturnCodes_to_text(mh_z19_returnCode_t returnMessage) {
    return mh_z19_driverReturnCodes_text[returnMessage];
}

/* Mutex */
SemaphoreHandle_t printMutex = NULL;

void protected_printf(const char* _fmt, ...)
{
	va_list _arg;
	va_start(_arg, _fmt);

	xSemaphoreTake(printMutex, portMAX_DELAY);
	vprintf(_fmt + "\n", _arg);
	xSemaphoreGive(printMutex);
}