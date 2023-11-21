/* add user code begin Header */
/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */
/* add user code end Header */

/* Includes ------------------------------------------------------------------*/
#include "at32f421_wk_config.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
#include "rk_usart.h"
#include "at32f421_board.h"
#include <string.h>
/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */

/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */

//#define USE_CUSTOM_HARDWARE_CRC16 

/* add user code end private define */

/* private macro -------------------------------------------------------------*/
/* add user code begin private macro */

/* add user code end private macro */

/* private variables ---------------------------------------------------------*/
/* add user code begin private variables */

/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */

RK_Usart_HandleTypeDef RKL_Uart2;
RK_Usart_Handles RK_Usart_Handle ={NULL};
static uint32_t table[256];
static uint8_t Data[8192];
/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */

#ifdef USE_CUSTOM_HARDWARE_CRC16
uint16_t RK_Usart_Crc16(uint8_t *ptr, uint16_t length)
{
  uint8_t _remainder =0;
  if (length < 1 ) return 0;
  if ((length %4) !=0 )
    { 
      memset(&ptr[length],0,3); 
      _remainder =1;
    }
  crc_data_reset();
  return  crc_block_calculate((uint32_t *)ptr, ( length / 4) + _remainder );
}
#endif


//用户Usart2接收回调函数
void MyUsart2_Rx(uint8_t * Data, uint16_t Size) 
{ 
  //这里处理收到的数据包
  
  RK_Usart_DMA_Transmit(RK_Usart_Phy_2,(uint8_t*)Data, Size);
  
  return;
}
  
//RK_Usart库初始化函数
void  Usart_Init(void)
{
  RKL_Uart2.PhyId = RK_Usart_Phy_2; //内部串口序列编号
  RKL_Uart2.usart = USART2;         //使用串口对象
  RKL_Uart2.tx_dma = DMA1_CHANNEL4; 
  RKL_Uart2.rx_dma = DMA1_CHANNEL5; 
  RKL_Uart2.RXCallback = MyUsart2_Rx; //设置用户RX接收回调函数
  RKL_Uart2.tx_crc = false; //CRC 校验和
  RKL_Uart2.rx_crc = false; //CCRC 校验和，校验失败则丢掉包
  RKL_Uart2.resend = false; //错误重发
  RKL_Uart2.tx_Size =128; //发送缓冲大小Byte
  RKL_Uart2.rx_Size =128; //接收缓冲大小Byte
  RK_Usart_Handle[RKL_Uart2.PhyId -1]= &RKL_Uart2; //添加对象结构到RKL_Uart管理句柄  
  
  RK_Usart_Init(&RK_Usart_Handle);
}


static uint32_t bitrev(uint32_t input, int bw)
{
    int i;
    uint32_t var;
    var = 0;
    for(i=0; i<bw; i++)
    {
        if(input & 0x01)
        {
            var |= 1<<(bw - 1 - i);
        }
        input >>= 1;
    }
    return var;
}

  
void crc32_init(uint32_t poly)
{
    int i;
    int j;
    uint32_t c;
 
 
    poly = bitrev(poly, 32);
    for(i=0; i<256; i++)
    {
        c = i;
        for (j=0; j<8; j++)
        {
            c = (c & 1) ? (poly ^ (c >> 1)) : (c >> 1);
        }
        table[i] = c;
    }
}


//计算CRC
uint32_t soft_crc32(uint32_t crc, void* input, int len)
{
    int i;
    uint8_t index;
    uint8_t *p;
    p = (uint8_t*)input;
    for(i=0; i<len; i++)
    {
        index = (*p ^ crc);
        crc = (crc >> 8) ^ table[index];
        p++;
    }
    return crc;

}

/* add user code end 0 */

/**
  * @brief main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  /* add user code begin 1 */

  /* add user code end 1 */

  /* system clock config. */
  wk_system_clock_config();

  /* config periph clock. */
  wk_periph_clock_config();

  /* nvic config. */
  wk_nvic_config();

  /* init dma1 channel4 */
  wk_dma1_channel4_init();
  /* config dma channel transfer parameter */
  /* user need to modify parameters memory_base_addr and buffer_size */
  wk_dma_channel_config(DMA1_CHANNEL4, (uint32_t)&USART2->dt, 0, 0);
  dma_channel_enable(DMA1_CHANNEL4, TRUE);

  /* init dma1 channel5 */
  wk_dma1_channel5_init();
  /* config dma channel transfer parameter */
  /* user need to modify parameters memory_base_addr and buffer_size */
  wk_dma_channel_config(DMA1_CHANNEL5, (uint32_t)&USART2->dt, 0, 0);
  dma_channel_enable(DMA1_CHANNEL5, TRUE);

  /* init usart2 function. */
  wk_usart2_init();

  /* init crc function. */
  wk_crc_init();

  /* add user code begin 2 */
  delay_init();
  
  Usart_Init();
  
  wk_gpio_config();
  
  char _putdata[] = "RK Usart library v0.1\r\n";
  RK_Usart_DMA_Transmit(RK_Usart_Phy_2,(uint8_t*)_putdata, strlen(_putdata));
  
  crc32_init(0x4C11DB7);
  uint32_t A_CRC =0;
  uint32_t B_CRC =0;
  gpio_bits_reset(GPIOA, GPIO_PINS_0);
  gpio_bits_set(GPIOA, GPIO_PINS_0);
  
  for (uint32_t i=0;i<1024;i++)
  {
    
   A_CRC =  crc_block_calculate((uint32_t *)&Data,2048 );
  }
  gpio_bits_reset(GPIOA, GPIO_PINS_0);
 
  delay_ms(200); 
   
  gpio_bits_set(GPIOA, GPIO_PINS_0);
  for (uint32_t i=0;i<1024;i++)
  {
    B_CRC = soft_crc32(0xFFFFFFFF, &Data, 8192);
  }
  gpio_bits_reset(GPIOA, GPIO_PINS_0);

  

 
  A_CRC = A_CRC +B_CRC; 
  
  /* add user code end 2 */

  while(1)
  {
    /* add user code begin 3 */
    
    // delay_ms(1000); 
    /* add user code end 3 */
  }
}
