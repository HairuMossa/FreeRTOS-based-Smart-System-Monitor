#include "main.h"
#include "stdint.h"
#include "rtos_system_status.h"
#include "rtos_logger.h"
#include "lis3dsh.h"
#include "rtos_tasks.h"
#include <string.h>


/**
 * variable to hold the received uart command from ISR
 * 0 - IDLE state
 * 1 - RUNNING state
 */
volatile uint8_t rxCommand = 0; // start with IDLE state


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){


    /*checking if the interrupt came from the button */
    if(GPIO_Pin == BUTTON_Pin){
    
    
    /*debounce logic*/ 
    uint32_t current_time = HAL_GetTick();

    if ((current_time - last_interrupt_time) > 200) 
    {
        CommandMessage_t txData;
        txData.id = 0x01;
        txData.uart_error = 0x00;

        last_interrupt_time = current_time;
            txData.value = 1; 

        /*put the command into Queue*/
        osMessageQueuePut (CommandQueueHandle, &txData, 0, 0);

    }

    } 
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    
  if(huart->Instance == USART2){
    CommandMessage_t txData; 
    txData.value = rxCommand;
    txData.id = 0x02;

    if(txData.value == '1'){ 
        txData.value = 1;
        txData.uart_error = 0x00;
    }
    else if(txData.value == '0'){ 
        txData.value = 0;
        txData.uart_error = 0x00;
    }
    else{
        txData.uart_error = 0x01;
    }

    
    
    /*put the command into Queue*/
    osMessageQueuePut (CommandQueueHandle, &txData, 0, 0);

    /**restarting the listener */
    HAL_UART_Receive_IT(&huart2, (uint8_t *)&rxCommand, 1);
  }
}

/* Pointer to generic accelerometer driver */
extern ACCELERO_DrvTypeDef Lis3dshDrv;
ACCELERO_DrvTypeDef *AcceleroDrv = &Lis3dshDrv;

void Acc_config_init(void){
  /* start the UART intterupt by passing the variable that will hold the received message*/
  HAL_UART_Receive_IT(&huart2, (uint8_t *)&rxCommand, 1);

  /* --------- Accelerometer (LIS3DSH) Init via generic driver --------- */

  uint8_t ctrl4 = 0;
  uint8_t ctrl5 = 0;
  uint16_t ctrl = 0;

  /* CTRL_REG4 configuration:
   * - Output data rate: 100 Hz
   * - Enable X, Y, Z axes
   *
   * These macros come from lis3dsh.h (ST driver).
   */
  ctrl4 |= LIS3DSH_DATARATE_100;   // ODR = 100 Hz
  ctrl4 |= LIS3DSH_XYZ_ENABLE;     // Enable X, Y, Z

  /* CTRL_REG5 configuration:
   * - ±2 g full scale
   * - 4-wire SPI
   * - Normal self-test (disabled)
   * - Anti-alias filter BW = 800 Hz
   */
  ctrl5 |= LIS3DSH_FULLSCALE_2;          // ±2 g
  ctrl5 |= LIS3DSH_SERIALINTERFACE_4WIRE;
  ctrl5 |= LIS3DSH_SELFTEST_NORMAL;
  ctrl5 |= LIS3DSH_FILTER_BW_800;

  /* Pack CTRL_REG4 (low byte) and CTRL_REG5 (high byte) into 16-bit value */
  ctrl  = (uint16_t)ctrl4;
  ctrl |= ((uint16_t)ctrl5 << 8);

  /* Initialize accelerometer using generic driver pointer */
  AcceleroDrv->Init(ctrl);

}

void ManagerFunction_Entry(void *argument)
{
  /* USER CODE BEGIN ManagerFunction */
  /* Infinite loop */

  CommandMessage_t rxData;
  
  for(;;)
  {
    
    if(osMessageQueueGet (CommandQueueHandle, &rxData,NULL, osWaitForever) == osOK){
        
        switch (rxData.id)
        {
        case 0x01: /*Button ISR*/
            /*toggle system state*/
            if(GetSystemState() == IDLE){
                SetSystemState(RUNNING);
            }
            else if(GetSystemState() == RUNNING){
                SetSystemState(IDLE);
            }
            LogPrint("Button Pressed!");
            break;
        case 0x02: /*UART ISR*/

            if(rxData.value == 1 && rxData.uart_error == 0x00){
                SetSystemState(RUNNING);
                LogPrint("UART Message Received: '1' ");
            } 
            else if(rxData.value == 0 && rxData.uart_error == 0x00){
                SetSystemState(IDLE);
                LogPrint("UART Message Received: '0' ");
            }
            else{
                LogPrint("Wrong UART Command! Please Press Either 1 or 0!");
            }
            break;

        default:
            break;
        }
    }
    
    osDelay(1);
  }
  /* USER CODE END ManagerFunction */
}

void PrintFunction_Entry(void *argument)
{
  /* USER CODE BEGIN PrintFunction */
  /* Infinite loop */
  LogMessage_t printMsg;
  for(;;)
  {
    if(osMessageQueueGet(PrintQueueHandle, &printMsg, NULL, osWaitForever) == osOK){
        
        /* Using blocking transmit is OK here because this Task is dedicated to it.*/ 
        HAL_UART_Transmit(&huart2, (uint8_t*)printMsg.buffer, strlen(printMsg.buffer), 100);
        
        // newline if needed
        HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 100);
    }
    osDelay(1);
  }
  /* USER CODE END PrintFunction */
}

void SensorFunction_Entry(void *argument)
{
    /* USER CODE BEGIN SensorFunction */
    /* Infinite loop */
    
    /* Raw and converted acceleration data */
    int16_t AccelRaw[3];   // X, Y, Z raw
    float   Accel_g[3];    // X, Y, Z in g
    
    /*configure accelerometer sensor*/
    Acc_config_init();
    uint8_t whoami = AcceleroDrv->ReadID();

    /*print the sensor ID , 0x3F*/
    LogPrint("Accelerometer ID(0x3F): %d", whoami);

    SystemState_e currentSystemState = GetSystemState();
    
    for(;;)
    {
        /*update current system state*/
        currentSystemState = GetSystemState();

        /* Read 3-axis acceleration via generic driver */
        AcceleroDrv->GetXYZ(AccelRaw);  // internally calls LIS3DSH_ReadACC()

        /* Convert raw counts to g */ 
        Accel_g[0] = (float)AccelRaw[0] / 1000.0f; // X in g
        Accel_g[1] = (float)AccelRaw[1] / 1000.0f; // Y in g
        Accel_g[2] = (float)AccelRaw[2] / 1000.0f; // Z in g

    /*for debugging*/
    for(int i=0; i<3; i++) {
        float val = Accel_g[i];

        /*Extract integer part (eg 0 from 0.98)*/ 
        int int_part = (int)val;

        /* Extract decimal part (eg 980 from 0.98)*/
        int dec_part = (int)((val - int_part) * 1000);

        /*Handle negative decimals*/
        if(dec_part < 0) dec_part = -dec_part;

        /* Print as "Integer.Decimal"*/
        LogPrint(" Accel %d : %d.%03d", i, int_part, dec_part);
    }

        /**
     * RED - POWER 
     * GREEN    - X Axis is between 0.9g and 1g and off otherwise
     * ORANGE   - Y Axis is between 0.9g and 1g and off otherwise
     * BLUE     - Z Axis is between 0.9g and 1g and off otherwise 
     */
    switch(currentSystemState){
        case IDLE:
            HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
            osDelay(100);
            HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
            osDelay(100);

            HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_ORANGE_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);

            LogPrint("IDLE\r\n");
            break;
        

        case RUNNING:
            HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);

            /* X Axis */
            if(Accel_g[0] >= 0.9 && Accel_g[0] <= 1){
                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
            }
            else{
                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
            }

            /* Y Axis */
            if(Accel_g[1] >= 0.9 && Accel_g[1] <= 1){
                HAL_GPIO_WritePin(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin, GPIO_PIN_SET);
            }
            else{
                HAL_GPIO_WritePin(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin, GPIO_PIN_RESET);
            }

            /* Z Axis */
            if(Accel_g[2] >= 0.9 && Accel_g[2] <= 1){
                HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
            }
            else{
                HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);
            }
            LogPrint("RUNNING\r\n");
            break;
    }    

    osDelay(500);
    }
  /* USER CODE END SensorFunction */
}