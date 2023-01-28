/**
 * @file fifo.c
 * @author HuangMiaozhi (hanoch1024@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2023-01-28
 *
 * @attention
 * Jinxin Microelec co.
 *
 */
#include "fifo.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

/**
 * @brief 注册一个fifo
 *
 * @param pfifo
 * @param pfifo_buf
 * @param size
 * @param lock
 * @param unlock
 */
void fifo_register(fifo_t *pfifo, uint8_t *pfifo_buf, uint32_t size, lock_fun lock, lock_fun unlock)
{
    pfifo->buf_size = size;
    pfifo->buf = pfifo_buf;
    pfifo->pwrite = pfifo->buf;
    pfifo->pread = pfifo->buf;
    pfifo->occupy_size = 0;
    pfifo->lock = lock;
    pfifo->unlock = unlock;
}

/**
 * @brief 释放FIFO
 *
 * @param pfifo
 */
void fifo_release(fifo_t *pfifo)
{
    pfifo->buf_size = 0;
    pfifo->buf = NULL;
    pfifo->pwrite = 0;
    pfifo->pread = 0;
    pfifo->occupy_size = 0;
    pfifo->lock = NULL;
    pfifo->unlock = NULL;
}

uint32_t fifo_write(fifo_t *pfifo, const uint8_t *pbuf, uint32_t size)
{
    uint32_t w_size = 0, free_size = 0;

    if ((size == 0) || (pfifo == NULL) || (pbuf == NULL)) // 如果这个FIFI不存在，则返回0
    {
        return 0;
    }

    free_size = fifo_get_free_size(pfifo); // 获取当前fifo剩余空间，如果剩余孔间为0则返回
    if (free_size == 0)
    {
        return 0;
    }

    if (free_size < size) // 如果传入fifo的size大于获取到的size，则更新fifo size
    {
        size = free_size;
    }
    w_size = size;
    if (pfifo->lock != NULL)
        pfifo->lock();
    while (w_size-- > 0)
    {
        *pfifo->pwrite++ = *pbuf++;
        if (pfifo->pwrite >= (pfifo->buf + pfifo->buf_size))
        {
            pfifo->pwrite = pfifo->buf;
        }
        pfifo->occupy_size++;
    }
    if (pfifo->unlock != NULL)
        pfifo->unlock();
    return size;
}

/**
 * @brief
 *
 * @param pfifo
 * @param pbuf
 * @param size
 * @return uint32_t
 */
uint32_t fifo_read(fifo_t *pfifo, uint8_t *pbuf, uint32_t size)
{
    uint32_t r_size = 0, occupy_size = 0;

    if ((size == 0) || (pfifo == NULL) || (pbuf == NULL))
    {
        return 0;
    }
    occupy_size = fifo_get_occupy_size(pfifo); // 获取已经使用的空间
    if (occupy_size == 0)
    {
        return 0;
    }

    if (occupy_size < size)
    {
        size = occupy_size;
    }
    if (pfifo->lock != NULL)
        pfifo->lock();
    r_size = size;
    while (r_size-- > 0)
    {
        /* code */
        *pbuf++ = *pfifo->pread++;
        if (pfifo->pread >= (pfifo->buf + pfifo->buf_size))
        {
            pfifo->pread = pfifo->buf;
        }
        pfifo->occupy_size--;
    }
    if (pfifo->unlock != NULL)
    {
        pfifo->unlock();
    }
    return size;
}
/**
 * @brief 获取fifo的空间大小
 *
 * @param pfifo
 * @return uint32_t
 */
uint32_t fifo_get_total_size(fifo_t *pfifo)
{
    if (pfifo == NULL)
        return 0;

    return pfifo->buf_size;
}

/**
 * @brief 获取fifo剩余空间大小
 * 
 * @param pfifo 
 * @return uint32_t 
 */
uint32_t fifo_get_free_size(fifo_t *pfifo)
{
    uint32_t size;

    if (pfifo == NULL)
        return 0;

    size = pfifo->buf_size - fifo_get_occupy_size(pfifo); // 总的fifo buf孔间减去已使用的fifo孔间

    return size;
}


/**
 * @brief 获取fifo已使用空间大小
 * 
 * @param pfifo 
 * @return uint32_t 
 */
uint32_t fifo_get_occupy_size(fifo_t *pfifo)
{
	if (pfifo==NULL)
		return 0;
    
	return  pfifo->occupy_size;
}
