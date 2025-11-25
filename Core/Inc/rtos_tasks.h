#ifndef __RTOS_TASKS_H
#define __RTOS_TASKS_H

extern osMessageQueueId_t CommandQueueHandle; /* CommandQueue*/

extern osMessageQueueId_t PrintQueueHandle; /*PrintQueue*/

extern UART_HandleTypeDef huart2;
static volatile uint32_t last_interrupt_time = 0; 



extern volatile uint8_t rxCommand;

typedef struct{
    /*ID to know which channel did the message comes from*/
    /*BUTTON ISR - ID = 0x01, UART ISR - ID = 0x02*/
    uint8_t id;
    uint8_t value; /* value can be either 1 or 0 */
    uint8_t uart_error; /* if wrong button is pressed, error = 0x01, else 0x00*/
}CommandMessage_t;

void Acc_config_init(void);
void ManagerFunction_Entry(void *argument);
void PrintFunction_Entry(void * arguument);
void SensorFunction_Entry(void *argument);


#endif // __RTOS_TASKS_H
