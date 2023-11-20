/**
  ******************************************************************************
  *
  * File Name   : rk_usart.c
  * Description : USART DMA receives and transmits, and idles interrupts 
  *               the receiving component.
  * Author      : Reverseking
  * MCU         : AT32F***
  * History     : [*] 2023/11/19 Created first version.             
    [#] Create
    [+] Features added
    [-] Features removed 
    [*] Features modification.
    
  ******************************************************************************
  *      Copyright (c) 2023 R.K   All rights reserved.
  ******************************************************************************
*/


/* Includes ------------------------------------------------------------------*/
#include "rk_usart.h"
#include "stdlib.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
 
/* USER CODE END Includes */


/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */


/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

const uint16_t CRC16_TAB[]= { 
	0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401, 
	0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400 
};

/* USER CODE END EM */


/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
RK_Usart_Handles * UartHandles;

/* USER CODE END Private defines */


/* USER CODE BEGIN -----------------------------------------------------------*/

__weak uint16_t RK_Usart_Crc16(uint8_t *ptr, uint16_t length)
{
  uint16_t crc = 0xffff; 
  uint16_t i;
  uint8_t ch;
  if (length < 1 ) return 0;
  for (i = 0; i < length; i++) 
  {
    ch = *ptr++;
    crc = CRC16_TAB[(ch ^ crc) & 15] ^ (crc >> 4);
    crc = CRC16_TAB[((ch >> 4) ^ crc) & 15] ^ (crc >> 4);
   } 
  crc = ((crc & 0x00FF) << 8) | ((crc & 0xFF00) >> 8); 
  
 return crc;
}


void RK_Usart_DMA_Transmit(RK_Usart_PhyId PhyId, uint8_t *pData, uint16_t Size)
{
  
  if ((PhyId < 1) || (PhyId > RK_USART_HANDLE_COUNT)) return;
  
  RK_Usart_HandleTypeDef * _handle = (*UartHandles)[PhyId -1];
  
  if (( Size < 1 ) || ( Size > (_handle->tx_Size + RK_USART_HEAD_SIZE))) return;

  
  while (_handle->busy)  {;}
    
  _handle-> tx_Count = Size;
  
  if (_handle->tx_crc )
  {
    RK_Usart_DataPackDef * _TxPack = (RK_Usart_DataPackDef *)_handle->txBuff;
    _TxPack->head = RK_USART_PACK_HEAD; 
    _TxPack->len = Size;
    _TxPack->crc  = RK_Usart_Crc16((uint8_t *)pData, Size); 
    memcpy(_TxPack->Buff ,pData, _handle->tx_Count); 
    _handle-> tx_Count+=6;
  } else
  {
    memcpy(_handle->txBuff ,pData, _handle->tx_Count);  
  }

#ifdef  ARTERY_AT32  
  while(usart_flag_get(_handle->usart, USART_TDBE_FLAG) == RESET);
  dma_channel_enable(_handle->tx_dma, FALSE);
  dma_data_number_set(_handle->tx_dma, _handle->tx_Count);
  dma_channel_enable(_handle->tx_dma, TRUE);
#endif
  
#if defined (ST_STM32F10X) || defined (ST_STM32F3XX) || defined (ST_STM32F4XX) || \
    defined (ST_STM32F0XX) || defined (ST_STM32L0XX)
    
  while ( _handle->usart->hdmatx->State != HAL_DMA_STATE_READY ) {;}
  HAL_UART_Transmit_DMA(_handle->usart , _handle->txBuff ,_handle->tx_Count); 
  
#endif  
}


void RK_Usart_TxError_Resend(RK_Usart_HandleTypeDef * handle)
{
  handle->busy = true;
  
#ifdef  ARTERY_AT32  
  while(usart_flag_get(handle->usart, USART_TDBE_FLAG) == RESET);
  dma_channel_enable(handle->tx_dma, FALSE);
  dma_data_number_set(handle->tx_dma, handle->tx_Count);
  dma_channel_enable(handle->tx_dma, TRUE);
#endif

#if defined (ST_STM32F10X) || defined (ST_STM32F3XX) || defined (ST_STM32F4XX) || \
    defined (ST_STM32F0XX) || defined (ST_STM32L0XX)
  while ( handle->usart->hdmatx->State != HAL_DMA_STATE_READY ) {;}
  HAL_UART_Transmit_DMA(handle->usart , handle->txBuff , handle->tx_Count); 
#endif  
  
  handle->busy = false;
  
}


void RK_Usart_RxError_Resend(RK_Usart_HandleTypeDef * handle)
{
  handle->busy = true; 
  RK_Usart_DataPackDef * _TxPack = (RK_Usart_DataPackDef *)handle->txBuff;
  _TxPack->head = RK_USART_CMD_RESEND; 
  _TxPack->len = 0;
#ifdef  ARTERY_AT32 
  while(usart_flag_get(handle->usart, USART_TDBE_FLAG) == RESET);
  dma_channel_enable(handle->tx_dma, FALSE);
  dma_data_number_set(handle->tx_dma, 4);
  dma_channel_enable(handle->tx_dma, TRUE);
#endif
  
#if defined (ST_STM32F10X) || defined (ST_STM32F3XX) || defined (ST_STM32F4XX)|| \
    defined (ST_STM32F0XX) || defined (ST_STM32L0XX)
    
  while ( handle->usart->hdmatx->State != HAL_DMA_STATE_READY ) {;}
  HAL_UART_Transmit_DMA(handle->usart , handle->txBuff , 4); 
#endif  
  
  handle->busy = false;
}


#ifdef  ARTERY_AT32 
void RK_Usart_RxCallback(RK_Usart_PhyId PhyId)
{
  if ((PhyId < 1) || (PhyId > RK_USART_HANDLE_COUNT)) return;  
  RK_Usart_HandleTypeDef * _handle = (*UartHandles)[PhyId -1];  
  dma_channel_enable(_handle->rx_dma , FALSE);
  if(usart_flag_get(_handle->usart , USART_IDLEF_FLAG) != RESET)
  {
     usart_data_receive(_handle->usart);   
     _handle->rx_Count = _handle->rx_Size + RK_USART_HEAD_SIZE - dma_data_number_get(_handle->rx_dma); 
     dma_data_number_set(_handle->rx_dma , _handle->rx_Size + RK_USART_HEAD_SIZE );
     if ( _handle->rx_Count > 0 && _handle->rx_Count < (_handle->rx_Size + RK_USART_HEAD_SIZE) )
     {
       uint16_t _Crc16 = 0;
       RK_Usart_DataPackDef * _RxPack = (RK_Usart_DataPackDef *)_handle->rxBuff;
       if (_RxPack->head == RK_USART_CMD_RESEND)
       { 
         RK_Usart_TxError_Resend(_handle);
         dma_channel_enable(_handle->rx_dma, TRUE);
         return;   
       }
       
       if (_handle->rx_crc) 
       { 
          if ( _handle->rx_Count > 6 )
             _Crc16 = RK_Usart_Crc16(_RxPack->Buff, _handle->rx_Count -6 ); //CRC16-ModBus
          
          if (_Crc16 != _RxPack->crc ) 
          { 
            if (_handle->resend) RK_Usart_RxError_Resend(_handle); 
            dma_channel_enable(_handle->rx_dma, TRUE);
            return; 
          } 
          
          if (_handle->RXCallback !=NULL)
           {
             _handle->RXCallback(_RxPack->Buff , _handle->rx_Count - 6 ); 
           } 
        } else
        {
          if (_handle->RXCallback !=NULL)
           {
             _handle->RXCallback(_handle->rxBuff , _handle->rx_Count );  
           }
        }
     }
  }
 
  dma_channel_enable(_handle->rx_dma, TRUE); 
    
}

#endif


#if defined (ST_STM32F10X) || defined (ST_STM32F3XX) || defined (ST_STM32F4XX) || \
    defined (ST_STM32F0XX) || defined (ST_STM32L0XX)
      
void RK_Usart_RxCallback(RK_Usart_PhyId PhyId)
{
  if ((PhyId < 1) || (PhyId > RK_USART_HANDLE_COUNT)) return;  
  
  RK_Usart_HandleTypeDef * _handle = (*UartHandles)[PhyId -1];  
  
  if((__HAL_UART_GET_FLAG(_handle->usart,UART_FLAG_IDLE) != RESET)) 
  {
     __HAL_UART_CLEAR_IDLEFLAG(_handle->usart); 
    
     _handle->rx_Count = _handle->rx_Size + RK_USART_HEAD_SIZE - __HAL_DMA_GET_COUNTER(_handle->usart->hdmarx); 
     HAL_UART_DMAStop(_handle->usart);
     if ( _handle->rx_Count > 0 && _handle->rx_Count < (_handle->rx_Size + RK_USART_HEAD_SIZE) )
     {
       uint16_t _Crc16 = 0;     
       RK_Usart_DataPackDef * _RxPack = (RK_Usart_DataPackDef *)_handle->rxBuff;
  
       if (_RxPack->head == RK_USART_CMD_RESEND)
       { 
         RK_Usart_TxError_Resend(_handle);
         HAL_UART_Receive_DMA(_handle->usart,_handle->rxBuff ,(_handle->rx_Size + RK_USART_HEAD_SIZE));  
         __HAL_UART_DISABLE_IT(_handle->usart, UART_IT_ERR);
         __HAL_UART_DISABLE_IT(_handle->usart, UART_IT_PE);
         __HAL_UART_CLEAR_OREFLAG(_handle->usart); 
         return;  
       }
       
       if (_handle->rx_crc) 
       { 
         
          if (_RxPack->len > 1 )
             _Crc16 = RK_Usart_Crc16(_RxPack->Buff, _RxPack->len ); //CRC16-ModBus
          
          if (_Crc16 != _RxPack->crc ) 
          { 
            if (_handle->resend) RK_Usart_RxError_Resend(_handle); 
            HAL_UART_Receive_DMA(_handle->usart,_handle->rxBuff ,_handle->rx_Size + RK_USART_HEAD_SIZE);  
            __HAL_UART_DISABLE_IT(_handle->usart, UART_IT_ERR);
            __HAL_UART_DISABLE_IT(_handle->usart, UART_IT_PE);
            __HAL_UART_CLEAR_OREFLAG(_handle->usart);
            return; 
          } 
          
          if (_handle->RXCallback !=NULL)
           {
             _handle->RXCallback(_RxPack->Buff , _handle->rx_Count - 6 );   
           } 
        } else
        {
          if (_handle->RXCallback !=NULL)
           {
             _handle->RXCallback(_handle->rxBuff , _handle->rx_Count );   
           }
        }
     }
     
     HAL_UART_Receive_DMA(_handle->usart,_handle->rxBuff ,_handle->rx_Size + RK_USART_HEAD_SIZE);  
     __HAL_UART_DISABLE_IT(_handle->usart, UART_IT_ERR);
     __HAL_UART_DISABLE_IT(_handle->usart, UART_IT_PE);
     __HAL_UART_CLEAR_OREFLAG(_handle->usart); 
   
  }
}
#endif 


bool RK_Usart_Config(void)
{
  bool _valid =false;
  for (uint16_t i =0;i< RK_USART_HANDLE_COUNT ;i++)
  {
    
    RK_Usart_HandleTypeDef * _handle = (*UartHandles)[i];
    
    if (_handle == NULL ) continue;
    _valid =true;
    
    _handle->txBuff = (uint8_t *)malloc(_handle->tx_Size + RK_USART_HEAD_SIZE);
    _handle->rxBuff = (uint8_t *)malloc(_handle->rx_Size + RK_USART_HEAD_SIZE);
    memset(_handle->txBuff,0,_handle->tx_Size + RK_USART_HEAD_SIZE); 
    memset(_handle->rxBuff,0,_handle->rx_Size + RK_USART_HEAD_SIZE); 
     
#ifdef  ARTERY_AT32  
     dma_channel_enable(_handle->tx_dma, FALSE);
     wk_dma_channel_config(_handle->tx_dma, (uint32_t)&_handle->usart->dt,(uint32_t)_handle->txBuff, 0);
     dma_channel_enable(_handle->tx_dma, TRUE);
    
     dma_channel_enable(_handle->rx_dma, FALSE);
     wk_dma_channel_config(_handle->rx_dma, (uint32_t)&_handle->usart->dt, (uint32_t)_handle->rxBuff, _handle->rx_Size + RK_USART_HEAD_SIZE);
     dma_channel_enable(_handle->rx_dma, TRUE);
 
     usart_flag_clear(_handle->usart,USART_IDLEF_FLAG);
     usart_interrupt_enable(_handle->usart, USART_IDLE_INT, TRUE); 
     
#endif
     
#if defined (ST_STM32F10X) || defined (ST_STM32F3XX) || defined (ST_STM32F4XX) || \
    defined (ST_STM32F0XX) || defined (ST_STM32L0XX)
    __HAL_UART_CLEAR_IDLEFLAG(_handle->usart);
    HAL_UART_Receive_DMA(_handle->usart,_handle->rxBuff,_handle->rx_Size + RK_USART_HEAD_SIZE);	
    __HAL_UART_ENABLE_IT(_handle->usart,UART_IT_IDLE);
#endif     
 
  }
  
 return _valid;  
}


bool RK_Usart_Init(RK_Usart_Handles *husart)
{
  if ( husart == NULL)  return false;
  
  UartHandles = husart;
 
  return RK_Usart_Config(); 

}


/* USER CODE END -----------------------------------------------------------*/


/************* (C) Copyright (c) 2023  R.K *****END OF FILE****/
