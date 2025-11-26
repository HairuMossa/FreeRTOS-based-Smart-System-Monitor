<<<<<<< HEAD
/**
 * @file lis3dsh_io.c
 * @author Hairu Mossa
 * @brief  LIS3DSH accelerometer hardware I/O interface and SPI communication.
 * @version 0.1
 * @date 2025-11-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */
=======
>>>>>>> ae402f51ded0bba1f645ea3db9f6517f5bdb5027
#include "main.h"
#include "lis3dsh.h"


extern SPI_HandleTypeDef hspi1;

#define CS_HIGH HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_SET)
#define CS_LOW  HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET)

<<<<<<< HEAD
/**
 * @brief Initializes the SPI interface for LIS3DSH accelerometer.
 * 
 */
=======

>>>>>>> ae402f51ded0bba1f645ea3db9f6517f5bdb5027
void ACCELERO_IO_Init(void){
    
    /*no need to do anything here since the SPI is already configured on main.c*/
     
}
<<<<<<< HEAD

/**
 * @brief Configures interrupt for LIS3DSH accelerometer.
 * 
 */
=======
>>>>>>> ae402f51ded0bba1f645ea3db9f6517f5bdb5027
void ACCELERO_IO_ITConfig(void){
    
}

<<<<<<< HEAD
/**
 * @brief Writes data to LIS3DSH accelerometer via SPI.
 * 
 * @param pBuffer Pointer to data buffer to write.
 * @param WriteAddr Register address to write to.
 * @param NumByteToWrite Number of bytes to write.
 */
void ACCELERO_IO_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
=======
void ACCELERO_IO_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
    /* bit7 = 0 → write
       bit6 = 1 → auto-increment for multi-byte */
                                                        // if (NumByteToWrite > 1)
                                                        // {
                                                        //     WriteAddr |= 0x40;    // set INC bit
                                                        // }

>>>>>>> ae402f51ded0bba1f645ea3db9f6517f5bdb5027
    CS_LOW;
    HAL_SPI_Transmit(&hspi1, &WriteAddr, 1, HAL_TIMEOUT);
    if (NumByteToWrite > 0)
    {
        HAL_SPI_Transmit(&hspi1, pBuffer, NumByteToWrite, HAL_TIMEOUT);
    }
    CS_HIGH;
}

<<<<<<< HEAD
/**
 * @brief Reads data from LIS3DSH accelerometer via SPI.
 * 
 * @param pBuffer Pointer to data buffer to store read data.
 * @param ReadAddr Register address to read from.
 * @param NumByteToRead Number of bytes to read.
 */
void ACCELERO_IO_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{
    ReadAddr |= 0x80;        // set READ bit in MSB
=======
void ACCELERO_IO_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{
    /* bit7 = 1 → read
       bit6 = 1 → auto-increment for multi-byte */
                                                    // if (NumByteToRead > 1)
                                                    // {
                                                    //     ReadAddr |= 0x40;    // set INC bit
                                                    // }
    ReadAddr |= 0x80;        // set READ bit in MSB

>>>>>>> ae402f51ded0bba1f645ea3db9f6517f5bdb5027
    CS_LOW;
    HAL_SPI_Transmit(&hspi1, &ReadAddr, 1, HAL_TIMEOUT);
    if (NumByteToRead > 0)
    {
        HAL_SPI_Receive(&hspi1, pBuffer, NumByteToRead, HAL_TIMEOUT);
    }
    CS_HIGH;
}

