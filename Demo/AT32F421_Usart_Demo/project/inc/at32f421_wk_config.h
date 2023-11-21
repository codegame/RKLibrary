/* add user code begin Header */
/**
  **************************************************************************
  * @file     at32f421_wk_config.h
  * @brief    header file of work bench config
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

/* define to prevent recursive inclusion -----------------------------------*/
#ifndef __AT32F421_WK_CONFIG_H
#define __AT32F421_WK_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes -----------------------------------------------------------------------*/
#include "at32f421.h"

/* private includes -------------------------------------------------------------*/
/* add user code begin private includes */

/* add user code end private includes */

/* exported types -------------------------------------------------------------*/
/* add user code begin exported types */

/* add user code end exported types */

/* exported constants --------------------------------------------------------*/
/* add user code begin exported constants */

/* add user code end exported constants */

/* exported macro ------------------------------------------------------------*/
/* add user code begin exported macro */

/* add user code end exported macro */

/* exported functions ------------------------------------------------------- */
  /* system clock config. */
  void wk_system_clock_config(void);

  /* config periph clock. */
  void wk_periph_clock_config(void);

  /* nvic config. */
  void wk_nvic_config(void);

  /* init crc function. */
  void wk_crc_init(void);

  /* init usart2 function. */
  void wk_usart2_init(void);

  /* init dma1 channel4 */
  void wk_dma1_channel4_init(void);

  /* init dma1 channel5 */
  void wk_dma1_channel5_init(void);

  /* config dma channel transfer parameter */
  /* user need to modify parameters memory_base_addr and buffer_size */
  void wk_dma_channel_config(dma_channel_type* dmax_channely, uint32_t peripheral_base_addr, uint32_t memory_base_addr, uint16_t buffer_size);

/* add user code begin exported functions */
void wk_gpio_config(void);
/* add user code end exported functions */

#ifdef __cplusplus
}
#endif

#endif
