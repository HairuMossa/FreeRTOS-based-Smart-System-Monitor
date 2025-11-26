/**
 * @file rtos_tasks.h
 * @author Hairu Mossa
 * @brief  Header file for RTOS task implementations.
 * @version 0.1
 * @date 2025-11-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef __RTOS_TASKS_H
#define __RTOS_TASKS_H

#include "main.h"
#include "rtos_system_status.h"
#include "rtos_logger.h"
#include "lis3dsh.h"
#include <string.h>


extern osMessageQueueId_t CommandQueueHandle; /* CommandQueue*/

extern osMessageQueueId_t PrintQueueHandle; /*PrintQueue*/

extern UART_HandleTypeDef huart2;

/*debounce variable*/
static volatile uint32_t last_interrupt_time = 0; 


extern volatile uint8_t rxCommand;

typedef struct{
    /*ID to know which channel did the message comes from*/
    /*BUTTON ISR - ID = 0x01, UART ISR - ID = 0x02*/
    uint8_t id;
    uint8_t value; /* value can be either 1 or 0 */
    uint8_t uart_error; /* if wrong button is pressed, error = 0x01, else 0x00*/
}CommandMessage_t;

/**
 * @brief Configures the accelerometer sensor.
 * 
 */
void Acc_config_init(void);

/**
 * @brief Entry function for Manager Task.
 * 
 * @param argument 
 */
void ManagerFunction_Entry(void *argument);

/**
 * @brief Entry function for Print Task.
 * 
 * @param arguument 
 */
void PrintFunction_Entry(void * arguument);

/**
 * @brief Entry function for Sensor Task.
 * 
 * @param argument 
 */
void SensorFunction_Entry(void *argument);


#endif // __RTOS_TASKS_H
