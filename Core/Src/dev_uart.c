/**
 * @file dev_uart.c
 * @author HuangMiaozhi (hanoch1024@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-28
 * 
 * @attention
 * Jinxin Microelec co.
 * 
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "usart.h"
#include "fifo.h"
#include "dev_uart.h"

#define UART1_TX_BUF_SIZE   256
#define UART1_RX_BUF_SIZE   128
#define UART1_DMA_RX_BUF_SIZE   128
#define UART1_DMA_TX_BUF_SIZE   64

#define 