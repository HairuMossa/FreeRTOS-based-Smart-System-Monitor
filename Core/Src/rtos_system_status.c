#include "rtos_system_status.h"
<<<<<<< HEAD
=======
#include "cmsis_os.h"
>>>>>>> ae402f51ded0bba1f645ea3db9f6517f5bdb5027


/*initialize system state with IDLE*/
/*hidden from other files so that it can't be modified outside this file*/
static volatile SystemState_e currentSystemState = IDLE;

<<<<<<< HEAD
=======
extern osMutexId_t SystemStateHandle;
>>>>>>> ae402f51ded0bba1f645ea3db9f6517f5bdb5027

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

