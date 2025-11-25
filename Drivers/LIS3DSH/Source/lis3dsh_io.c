#include "main.h"
#include "lis3dsh.h"


extern SPI_HandleTypeDef hspi1;

#define CS_HIGH HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_SET)
#define CS_LOW  HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET)


void ACCELERO_IO_Init(void){
    
    /*no need to do anything here since the SPI is already configured on main.c*/
     
}
void ACCELERO_IO_ITConfig(void){
    
}

void ACCELERO_IO_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
    /* bit7 = 0 → write
       bit6 = 1 → auto-increment for multi-byte */
                                                        // if (NumByteToWrite > 1)
                                                        // {
                                                        //     WriteAddr |= 0x40;    // set INC bit
                                                        // }

    CS_LOW;
    HAL_SPI_Transmit(&hspi1, &WriteAddr, 1, HAL_TIMEOUT);
    if (NumByteToWrite > 0)
    {
        HAL_SPI_Transmit(&hspi1, pBuffer, NumByteToWrite, HAL_TIMEOUT);
    }
    CS_HIGH;
}

void ACCELERO_IO_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{
    /* bit7 = 1 → read
       bit6 = 1 → auto-increment for multi-byte */
                                                    // if (NumByteToRead > 1)
                                                    // {
                                                    //     ReadAddr |= 0x40;    // set INC bit
                                                    // }
    ReadAddr |= 0x80;        // set READ bit in MSB

    CS_LOW;
    HAL_SPI_Transmit(&hspi1, &ReadAddr, 1, HAL_TIMEOUT);
    if (NumByteToRead > 0)
    {
        HAL_SPI_Receive(&hspi1, pBuffer, NumByteToRead, HAL_TIMEOUT);
    }
    CS_HIGH;
}

