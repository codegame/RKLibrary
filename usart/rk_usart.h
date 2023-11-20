/**
  ******************************************************************************
  *
  * File Name   : rk_usart.h
  * Description : USART DMA receives and transmits, and idles interrupts 
  *               the receiving component.
  * Author      : Reverseking
  * MCU         : AT32Fxxx  STM32Fxxx
  * History     : [*] 2023/11/20 Created first version.             
    [#] Create
    [+] Features added
    [-] Features removed 
    [*] Features modification.


Features:
**1.收发使用DMA和空闲中断**
**2.支持不定长数据发送**
**3.支持最多16个串口同时工作**
**4.支持收发CRC16透明校验**
**5.支持自定义CRC16校验算法**
**6.支持校验错误自动请求重发**
**7.支持自定义缓冲区大小**

    
  ******************************************************************************
  *      Copyright (c) 2023 R.K  All rights reserved.
  ******************************************************************************
*/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __rk_usart_H
#define __rk_usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "rk_type_def.h"
#if defined (AT32F421C8T7)   || defined (AT32F421K8T7) || defined (AT32F421K8U7) || \
    defined (AT32F421K8U7_4) || defined (AT32F421F8P7) || defined (AT32F421G8U7) || \
    defined (AT32F421C6T7)   || defined (AT32F421K6T7) || defined (AT32F421K6U7) || \
    defined (AT32F421K6U7_4) || defined (AT32F421F6P7) || defined (AT32F421G6U7) || \
    defined (AT32F421C4T7)   || defined (AT32F421K4T7) || defined (AT32F421K4U7) || \
    defined (AT32F421K4U7_4) || defined (AT32F421F4P7) || defined (AT32F421G4U7) || \
    defined (AT32F421PF8P7)  || defined (AT32F421PF4P7)|| defined (AT32F4212C8T7)

 #include "at32f421_wk_config.h"  
 #define  ARTERY_AT32    
#endif
  
#if defined (AT32F423K8U7_4) || defined (AT32F423KBU7_4) || defined (AT32F423KCU7_4) || \
    defined (AT32F423T8U7)   || defined (AT32F423TBU7)   || defined (AT32F423TCU7)   || \
    defined (AT32F423C8U7)   || defined (AT32F423CBU7)   || defined (AT32F423CCU7)   || \
    defined (AT32F423C8T7)   || defined (AT32F423CBT7)   || defined (AT32F423CCT7)   || \
    defined (AT32F423R8T7_7) || defined (AT32F423RBT7_7) || defined (AT32F423RCT7_7) || \
    defined (AT32F423R8T7)   || defined (AT32F423RBT7)   || defined (AT32F423RCT7)   || \
    defined (AT32F423V8T7)   || defined (AT32F423VBT7)   || defined (AT32F423VCT7)   || \
    defined (AT32F423CBU7_7)
      
 #include "at32f423_wk_config.h"
 #define  ARTERY_AT32   
#endif

      
      
#if defined (STM32L010x4) || defined (STM32L010x6) || defined (STM32L010x8) || \
    defined (STM32L011xx) || defined (STM32L021xx) || defined (STM32L010xB) || \
    defined (STM32L031xx) || defined (STM32L041xx) || defined (STM32L083xx) || \
    defined (STM32L051xx) || defined (STM32L052xx) || defined (STM32L053xx) || \
    defined (STM32L062xx) || defined (STM32L063xx) || defined (STM32L082xx) || \
    defined (STM32L071xx) || defined (STM32L072xx) || defined (STM32L073xx) || \
    defined (STM32L081xx)
      
 #include "usart.h"
 #define  ST_STM32L0XX        
#endif
      
#if defined (STM32F030x6) || defined (STM32F030x8) || defined (STM32F070xB) || defined (STM32F071xB) || \
    defined (STM32F031x6) || defined (STM32F038xx) || defined (STM32F078xx) || defined (STM32F072xB) || \
    defined (STM32F042x6) || defined (STM32F048xx) || defined (STM32F070x6) || defined (STM32F091xC) || \
    defined (STM32F051x8) || defined (STM32F058xx) || defined (STM32F030xC) || defined (STM32F098xx)
      
 #include "usart.h"
 #define  ST_STM32F0XX        
#endif
        
      
#if defined (STM32F100xB) || defined (STM32F100xE) || defined (STM32F101x6) || \
    defined (STM32F101xB) || defined (STM32F101xE) || defined (STM32F101xG) || \
    defined (STM32F102x6) || defined (STM32F102xB) || defined (STM32F103x6) || \
    defined (STM32F103xB) || defined (STM32F103xE) || defined (STM32F103xG) || \
    defined (STM32F105xC) || defined (STM32F107xC)
 #include "usart.h"
 #define  ST_STM32F10X        
#endif
      
      
#if defined (STM32F301x8) || defined (STM32F302x8) || defined (STM32F318xx) || \
    defined (STM32F302xC) || defined (STM32F303xC) || defined (STM32F358xx) || \
    defined (STM32F303x8) || defined (STM32F334x8) || defined (STM32F328xx) || \
    defined (STM32F302xE) || defined (STM32F303xE) || defined (STM32F398xx) || \
    defined (STM32F373xC) || defined (STM32F378xx)    
 #include "usart.h"
 #define  ST_STM32F3XX        
#endif 
     
#if defined (STM32F405xx) || defined (STM32F415xx) || defined (STM32F407xx) || defined (STM32F417xx) || \
    defined (STM32F427xx) || defined (STM32F437xx) || defined (STM32F429xx) || defined (STM32F439xx) || \
    defined (STM32F401xC) || defined (STM32F401xE) || defined (STM32F410Tx) || defined (STM32F410Cx) || \
    defined (STM32F410Rx) || defined (STM32F411xE) || defined (STM32F446xx) || defined (STM32F469xx) || \
    defined (STM32F479xx) || defined (STM32F412Cx) || defined (STM32F412Rx) || defined (STM32F412Vx) || \
    defined (STM32F412Zx) || defined (STM32F413xx) || defined (STM32F423xx)
      
 #include "usart.h"
 #define  ST_STM32F4XX        
#endif    
      
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define  RK_USART_HEAD_SIZE             6       //包头大小 head  + len + crc =6byte
#define  RK_USART_HANDLE_COUNT          16      //最大支持串口序列
#define  RK_USART_CMD_RESEND            0x3FF3  //重发指令
#define  RK_USART_PACK_HEAD             0xEE77  //head CRC16计算
/* USER CODE END Includes */


/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
   
typedef enum {RK_Usart_Phy_1=1,RK_Usart_Phy_2,RK_Usart_Phy_3,RK_Usart_Phy_4,RK_Usart_Phy_5,RK_Usart_Phy_6,
              RK_Usart_Phy_7,RK_Usart_Phy_8,RK_Usart_Phy_9,RK_Usart_Phy_10,RK_Usart_Phy_11,RK_Usart_Phy_12,
              RK_Usart_Phy_13,RK_Usart_Phy_14,RK_Usart_Phy_15,RK_Usart_Phy_16} RK_Usart_PhyId; //Usart 编号

 
typedef void(*RK_Usart_Callback)(uint8_t * Data ,uint16_t); //回调函数定义

 
typedef struct 
{ 
  uint16_t head;
  uint16_t len;
  uint16_t crc;
  uint8_t  Buff[];
}RK_Usart_DataPackDef; 
 

typedef struct 
{ 
    RK_Usart_PhyId PhyId;  //串口编号 必须大于0
#ifdef  ARTERY_AT32  
    usart_type* usart;     //串口
    dma_channel_type* tx_dma;  //TX DMA
    dma_channel_type* rx_dma;  //RX DMA
#endif 

#if defined (ST_STM32L0XX) || defined (ST_STM32F0XX) || defined (ST_STM32F10X) || \
    defined (ST_STM32F4XX) || defined (ST_STM32F3XX) 
     
    UART_HandleTypeDef *usart; //串口
#endif 
    
    bool  tx_crc;       //TX CRC16 
    bool  rx_crc;       //RX CRC16  
    bool  busy;         //Usart忙碌
    bool  resend;       //如果CRC16校验错误resend>0则重发resend次数
    uint16_t tx_Count;  //最后一次发送长度
    uint16_t rx_Count;  //最后一次接收长度
    RK_Usart_Callback RXCallback;
    uint16_t tx_Size;   //Tx缓冲大小
    uint16_t rx_Size;   //Rx缓冲大小
    uint8_t * txBuff;   //DMA transmit buffer 
    uint8_t * rxBuff;   //DMA receive buffer
} RK_Usart_HandleTypeDef; 

typedef  RK_Usart_HandleTypeDef *RK_Usart_Handles[RK_USART_HANDLE_COUNT] ;  //Usart_Handle对象管理表

/* USER CODE END ET */

/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */
bool RK_Usart_Init(RK_Usart_Handles *husart); //初始化
void RK_Usart_DMA_Transmit(RK_Usart_PhyId PhyId, uint8_t *pData, uint16_t Size); //DMA发送
void RK_Usart_RxCallback(RK_Usart_PhyId PhyId); //添加到xx_it.c中断文件IRQHandler函数里
uint16_t RK_Usart_Crc16(uint8_t *ptr, uint16_t length); //提供用户重载CRC16算法

/* USER CODE END EFP */


/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */


#ifdef __cplusplus
}
#endif

#endif /* __rk_usart_H */


/************* (C) Copyright (c) 2023 R.K *****END OF FILE****/
