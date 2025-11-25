#ifndef __RTOS_LOGGER_H
#define __RTOS_LOGGER_H

#include "cmsis_os.h"

#define MAX_LOG_SIZE            64

typedef struct{
    char buffer[MAX_LOG_SIZE];
}LogMessage_t;

extern osMessageQueueId_t PrintQueueHandle;


void LogPrint(const char *format, ...); // Variadic function

#endif // __RTOS_LOGGER_H