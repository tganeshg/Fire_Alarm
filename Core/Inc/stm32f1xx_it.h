/**********************************************************************************
* File           : stm32f1xx_it.h				  							  	  *
* Brief          : 					  					  						  *
***********************************************************************************
*															  					  *
***********************************************************************************
/--File Histroy ------------------------------------------------------------------/
*	Date			Version		Name	Description		  						  *
/---------------------------------------------------------------------------------/
*	25-Nov-2020		01.00		GAN	Initial Development.	  					  *
**********************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F1xx_IT_H
#define __STM32F1xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported functions prototypes ---------------------------------------------*/
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void DebugMon_Handler(void);
void TIM2_IRQHandler(void);
void USART1_IRQHandler(void);
void ETH_IRQHandler(void);


#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_IT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
