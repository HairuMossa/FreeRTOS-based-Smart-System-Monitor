#ifndef __RTOS_SYSTEM_STATUS_H
#define __RTOS_SYSTEM_STATUS_H

#include "main.h"
typedef enum{
    IDLE,
    RUNNING
}SystemState_e;

SystemState_e GetSystemState(void);
void SetSystemState(SystemState_e newState);

#endif /* __RTOS_SYSTEM_STATUS_H */