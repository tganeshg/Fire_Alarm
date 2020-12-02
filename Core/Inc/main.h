/**********************************************************************************
* File           : main.h														  *
* Brief          : Main program body header										  *
***********************************************************************************
*						M	A	I	N	.	H									  *
***********************************************************************************
/--File Histroy ------------------------------------------------------------------/
*	Date			Version		Name		Description							  *
/---------------------------------------------------------------------------------/
*	25-Nov-2020		01.00		GAN			Initial Development.				  *
**********************************************************************************/

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "ds1307.h"
#include "stm32f1xx_hal.h"

/* Global Macros */

/* Structures */

/* Function Declaration */
DS1307_RESULT confDS1307(void);
void set_time(void);
void ethernetTask(void *argument);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

void ErrorHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/****************************** (C) COPYRIGHT @ ICONT *****************************/
/* EOF */
