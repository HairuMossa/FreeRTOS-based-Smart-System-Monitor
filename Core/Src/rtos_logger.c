#include "rtos_logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "main.h" 
#include "rtos_logger.h"

extern UART_HandleTypeDef huart2;

void LogPrint(const char *format, ...){

    LogMessage_t msg;
    
    /* Initialize variable argument list*/
    va_list args;
    va_start(args, format);

    /*Format the string safely into the struct's buffer
    vsnprintf ensures we don't overflow MAX_LOG_SIZE*/
    vsnprintf(msg.buffer, MAX_LOG_SIZE, format, args);

    /*Clean up args*/
    va_end(args);

    /*Send to Queue*/
    osMessageQueuePut(PrintQueueHandle, &msg, 0, 0);
}

#include "rtos_logger.h"

extern UART_HandleTypeDef huart2;

void LogPrint(const char *format, ...){

    LogMessage_t msg;
    
    /* Initialize variable argument list*/
    va_list args;
    va_start(args, format);

    /*Format the string safely into the struct's buffer
    vsnprintf ensures we don't overflow MAX_LOG_SIZE*/
    vsnprintf(msg.buffer, MAX_LOG_SIZE, format, args);

    /*Clean up args*/
    va_end(args);

    /*Send to Queue*/
    osMessageQueuePut(PrintQueueHandle, &msg, 0, 0);
}
