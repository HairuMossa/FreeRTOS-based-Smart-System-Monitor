<<<<<<< HEAD

/**
 * @file rtos_logger.h
 * @author Hairu Mossa
 * @brief   A simple RTOS logging utility using message queues.
            It defines a LogMessage_t struct and a LogPrint function to format and enqueue log messages.
            The PrintTask can then dequeue and print these messages via UART.
            This helps decouple logging from other tasks and ISRs in an RTOS environment.
 * @version 0.1
 * @date 2025-11-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */
=======
>>>>>>> ae402f51ded0bba1f645ea3db9f6517f5bdb5027
#ifndef __RTOS_LOGGER_H
#define __RTOS_LOGGER_H

#include "cmsis_os.h"
<<<<<<< HEAD
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "main.h"

/**
 * @brief Maximum size for log messages.
 * 
 */
#define MAX_LOG_SIZE            64

/**
 * @brief Log message structure to hold formatted log strings.
 * 
 */
=======

#define MAX_LOG_SIZE            64

>>>>>>> ae402f51ded0bba1f645ea3db9f6517f5bdb5027
typedef struct{
    char buffer[MAX_LOG_SIZE];
}LogMessage_t;

<<<<<<< HEAD

/**
 * @brief Handle for the print message queue.
 * 
 */
extern osMessageQueueId_t PrintQueueHandle;

/**
 * @brief Variadic function to format and enqueue log messages.
 * 
 * @param format Format string (like printf).
 * @param ... Variable arguments for formatting.
 */
=======
extern osMessageQueueId_t PrintQueueHandle;


>>>>>>> ae402f51ded0bba1f645ea3db9f6517f5bdb5027
void LogPrint(const char *format, ...); // Variadic function

#endif // __RTOS_LOGGER_H