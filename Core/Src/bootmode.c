/**
 * @file bootmode.c
 * @author HuangMiaozhi (hanoch1024@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2023-02-01
 *
 * @attention
 * Jinxin Microelec co.
 *
 */
//* 本.c文件主要进行
#include <string.h>
#include <stdlib.h>
#define BUFFERSIZE 255
uint8_t UartReceiveBuff[BUFFERSIZE];
uint8_t BootBuff[BUFFERSIZE];

extern UART_HandleTypeDef huart1;

// Todo 串口接收标志不应该是0或1吗  为什么用uint32_t?
volatile uint32_t UartRxFlag = 0;   //! 数据更新标志，在HAL_UART_RxCpltCallback中增加，由用户访问串口接收缓冲区后减小
volatile uint32_t BootBuffSize = 0; //! 从uart中更新的数据量
void MainLoop(void)
{
  bool BootMode = false;
  HAL_UART_Receive_DMA(&huart1, UartReceiveBuff, BUFFERSIZE); //     DMA
  while (1)
  {
    // TODO TimeOut100ms 如何确定32是以100ms的计算速度递减?
    int TimeOut100ms = 30;
    while ((TimeOut100ms--) > 0) // 3s内
    {
      if (UartRxFlag > 0) // 如果串口标志位有数据
      {
        UartRxFlag--;

        if (BootBuffSize >= 3) // 如果从串口缓冲区更新到boot数据缓冲区的大小>3
        {
          BootMode = true;

          if (BootBuff[0] == 0xFE && BootBuff[1] == 0xA5) // 判断是否是数据接收错误
          {
            bootcmd(BootBuff[2]);
          }
        }
      }
      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    }
    if ((!BootMode) && CrcVerify())
    {
      CmdJumpToApp();
    }
  }
}

/**
 * @brief boot模式下命令行显示
 *
 */
void bootcmd(uint8_t cmd)
{
  switch (cmd)
  {
  case 0x1A /* constant-expression */:
    /* code */
    CmdConnection(); //
    break;
  case 0xFF:
    CmdEraseFlash();
    break;
  case 0x2A:
    CmdFlashWrite();
    break;
  case 0x3A:
    CrcVerify();
    break;
  case 0xBB:
    CmdReset();
    break;
  case 0x4A:
    CmdJumpToApp();
    break;
  case 0x5A:
    CrcSuccessedAndJumpToApp();
    break;
  case 0x6A:
    CmdDirectJumpToApp();
    break;
  default:
    break;
  }
}

/**
 * @brief 测试串口连接
 *
 */
static void CmdConnection(void)
{
  printf("-----------------Connection----------------\r\n");
  uint8_t tmp[] = {0xAA, 0xBB, 0xA1, 0xA2, 0xA3, 0xA4};
  HAL_UART_Transmit(&huart1, tmp, sizeof(tmp) / sizeof(tmp[0]), 0xFFFF);
}

static void CmdEraseFlash()
{

}

static void CmdFLashWrite()
{
  
}