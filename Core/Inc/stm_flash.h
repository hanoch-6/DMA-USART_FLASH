/**
 * @file stm_flash.h
 * @author HuangMiaozhi (hanoch1024@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-28
 * 
 * @attention
 * Jinxin Microelec co.
 * 
 */
 #ifndef __STM_FLASH_H__
#define __STM_FLASH_H__

#include"stm32f1xx_hal.h"


typedef enum
{
    FAILED = 0,
    PASSED = !FAILED
}Status;

#define FLASH_WriteAddress  0x08001400
#define FLASH_ReadAddress   FLASH_WriteAddress
#define FLASH_SIZE      128     //128K


uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);
Status Buffercmp(uint16_t *pBuffer1, uint16_t *pBuffer2, uint16_t BufferLength);

void STMFLASH_Read(uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead);
void STMFLASH_Write(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite);


#endif