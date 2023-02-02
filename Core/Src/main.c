/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "stdio.h"
#include "stm_flash.h"

#ifdef __GNUC__

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)

#else

#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

#endif /* __GNUC__ */

#define BUFFERSIZE 255
#define FLASH_WriteAddress 0x08001400
#define FLASH_TESTSIZE 64

uint16_t Tx_Buffer[FLASH_TESTSIZE] = {0};
uint16_t Rx_Buffer[FLASH_TESTSIZE] = {0};

uint8_t UartReceiveBuff[BUFFERSIZE];
uint8_t RX_len;
volatile uint8_t receive_end_flag;
volatile Status TransferStatus = FAILED;

extern DMA_HandleTypeDef hdma_usart1_rx;
extern UART_HandleTypeDef huart1;

int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart1)
  {
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); //     C13  ?  ?
    // flag=1;
  } //
}
/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config(void);

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  uint16_t i;
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE); //        ?    ??
  MX_USART2_UART_Init();

  for (i = 0; i < FLASH_TESTSIZE; ++i)
  {
    Tx_Buffer[i] = i;
    Rx_Buffer[i] = 0;
  }
  printf("这是一个内部flash读写测试实验\n");

  STMFLASH_Write(FLASH_WriteAddress, Tx_Buffer, FLASH_TESTSIZE);

  HAL_Delay(1000);

  STMFLASH_Read(FLASH_ReadAddress, Rx_Buffer, FLASH_TESTSIZE);
  HAL_Delay(1000);
  TransferStatus = Buffercmp(Tx_Buffer, Rx_Buffer, FLASH_TESTSIZE);

  if (PASSED == TransferStatus)
  {
    printf("内部Flash测试成功\r\n");
    
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, SET);
  }
  else
  {
    printf("内部flash测试失败\r");
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, RESET);
  }
  uint32_t sysclock = 0;
  sysclock = HAL_RCC_GetSysClockFreq();

  printf("当前时钟频率sysclock=%u", sysclock);
  while (1)
  {
    printf("接收到的数据：%02d\r\n", UartReceiveBuff[i]);
    HAL_Delay(1000);
    // if (!receive_end_flag)
    //{
    //   for(int i = 0;i<strlen(ReceiveBuff);i++)
    //     printf("RECEIVE DATA  %02d\r\n",ReceiveBuff[i]);
    //}
  }
  /* USER CODE END 3 */
}

/**
 * @brief ? ? ?
 *
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}
/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
