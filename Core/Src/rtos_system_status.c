#include "rtos_system_status.h"
#include "cmsis_os.h"


/*initialize system state with IDLE*/
/*hidden from other files so that it can't be modified outside this file*/
static volatile SystemState_e currentSystemState = IDLE;

extern osMutexId_t SystemStateHandle;

SystemState_e GetSystemState(void){
    SystemState_e safeRead;
    
    /*waiting until we get the key*/
    if(osMutexAcquire(SystemStateHandle, osWaitForever) == osOK){
        safeRead = currentSystemState; 
        /* release the key*/
        osMutexRelease(SystemStateHandle);
    }
    return safeRead;
}
void SetSystemState(SystemState_e newState){
    /*take the key*/
    if(osMutexAcquire(SystemStateHandle, osWaitForever) == osOK){
        currentSystemState = newState; 
        /*release the key*/
        osMutexRelease(SystemStateHandle);
    }
    
}

