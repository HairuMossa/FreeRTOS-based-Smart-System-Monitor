/**
 * @file rtos_system_status.h
 * @author Hairu Mossa
 * @brief  A simple RTOS system state manager.
 *         It defines SystemState_e enum and provides GetSystemState and SetSystemState functions
 *         to safely read and modify the system state using a mutex (SystemStateHandle).
 * @version 0.1
 * @date 2025-11-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef __RTOS_SYSTEM_STATUS_H
#define __RTOS_SYSTEM_STATUS_H

#include "main.h"
#include "cmsis_os.h"

/**
 * @brief Enumeration for system states.
 * 
 */
typedef enum{
    IDLE,
    RUNNING
}SystemState_e;

extern osMutexId_t SystemStateHandle;

/**
 * @brief Handle for the system state mutex.
 * 
 */
SystemState_e GetSystemState(void);

/**
 * @brief Safely set the system state.
 * 
 * @param newState The new system state to set.
 */
void SetSystemState(SystemState_e newState);

#endif /* __RTOS_SYSTEM_STATUS_H */