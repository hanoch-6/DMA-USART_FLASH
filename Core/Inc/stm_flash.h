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
typedef enum
{
    FAILED = 0,
    PASSED = !FAILED
}Status;

#define FLASH_WriteAddress  0x0801FC00
#define FLASH_ReadAddress   FLASH_WriteAddress
#define FLASH_SIZE      128     //128K

volatile Status TransferStatus = FAILED;