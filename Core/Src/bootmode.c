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
//*  DMA接收中断开启，进入主循环
void doLoop(void)
{
    bool BootMode = false;
      HAL_UART_Receive_DMA(&huart1, ReceiveBuff, BUFFERSIZE); //     DMA   
    while(1)
    {
        int BootTime100Out = 30;
        if()
    }
}