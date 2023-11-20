/**
  ******************************************************************************
  *
  * File Name   : rk_type_def.h
  * Description : Variable type definition
  * Author      : Reverseking
  * History     : [*] 2023/11/19 Created first version.             
    [#] Create
    [+] Features added
    [-] Features removed 
    [*] Features modification.
    
  ******************************************************************************
  *      Copyright (c) 2023 SEIICHI technologies  All rights reserved.
  ******************************************************************************
*/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __rk_type_def_H
#define __rk_type_def_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */


/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */


/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */


/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
typedef char bool;
#define true 1
#define false 0

#define BitSet(V,B) V|=(1<<B)  //将V的第B位置1
#define BitRst(V,B) V&=~(1<<B) //将V的第B位清0 

/* USER CODE END EM */


/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */


/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */


#ifdef __cplusplus
}
#endif

#endif /* __rk_type_def_H */


/************* (C) Copyright (c) 2023 SEIICHI technologies *****END OF FILE****/
