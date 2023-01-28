/**
 * @file stm_flash.c
 * @author HuangMiaozhi (hanoch1024@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2023-01-28
 *
 * @attention
 * Jinxin Microelec co.
 *
 */

#include "stm32f1xx_hal.h"
#include "usart.h"
#include "stm_flash.h"

static Status Buffercmp(uint16_t *pBuffer1, uint16_t *pBuffer2, uint16_t BufferLength);

#define STM_PAGE_SIZE 1024
#define STM_FLASH_SIZE 128
static uint16_t STMFLASH_BUF[STM_PAGE_SIZE / 2];
static FLASH_EraseInitTypeDef EraseInitStruct;

//**1K 有1024个字节 stm32 一个字是32bit 半字就是16bit
/**
 * @brief 读取指定地址的半字
 *
 * @param faddr
 * @return uint16_t
 */
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr)
{
    return *(volatile uint16_t *)faddr;
}

void STMFLASH_Write(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite)
{
    uint32_t PAGE_Error = 0;
    uint16_t page_offset; // 页内偏移地址
    uint16_t page_remain; // 剩余地址
    uint16_t i;
    uint32_t page_pos; // 页的地址
    uin32_t offset;    // 去掉0x08000000后的地址

    if (WriteAddr < FLASH_BASE || (WriteAddr >= (FLASH_BASE + 1024 * STM_FLASH_SIZE)))
        return;

    HAL_FLASH_Unlock(); // 解锁

    offset = WriteAddr - FLASH_BASE; // 实际偏移地址
    page_pos = offset / STM_PAGE_SIZE;//页地址 0~31 for STM32F103C8T6
    
}