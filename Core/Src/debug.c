/**********************************************************************************
* File           : debug.c							  							  *
* Brief          : Debug related code						  					  *
***********************************************************************************
*							D	E	B	U	G	.	C		  					  *
***********************************************************************************
/--File Histroy ------------------------------------------------------------------/
*	Date			Version		Name	Description		  						  *
/---------------------------------------------------------------------------------/
*	25-Nov-2020		01.00		GAN	Initial Development.	  					  *
**********************************************************************************/

/* Includes */
#include "common.h"
#include "main.h"


/* Global Variables */
char dbgBuf[SIZE_1024];

/* Structure variables */
UART_HandleTypeDef pHuart;

/*** Function  Definations ***/

int uPrintf(const char *format, ...)
{
	va_list arg;
	uint32_t tmpSlen = 0;

	volatile unsigned int done;
	memset(dbgBuf,0,sizeof(dbgBuf));
	va_start (arg, format);
	done = vsprintf(dbgBuf, format, arg);
	tmpSlen = strlen(dbgBuf);
	HAL_UART_Transmit(&pHuart,(uint8_t*)dbgBuf,tmpSlen ,50);
	va_end(arg);
	return done;
}

/****************************** (C) COPYRIGHT @ ICONT *****************************/
/* EOF */
