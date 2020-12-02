/**********************************************************************************
* File           : common.h
* Brief          : Common defines
***********************************************************************************
*						C	O	M	M	O	N	.	H							  *
***********************************************************************************
/--File Histroy ------------------------------------------------------------------/
*	Date			Version		Name		Description
/---------------------------------------------------------------------------------/
*	25-Nov-2020		01.00		GAN			Initial Development.
**********************************************************************************/

#ifndef __COMMON_H
#define __COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* Global Macros */

//* SIZES *//
#define	SIZE_2		2
#define	SIZE_4		4
#define	SIZE_8		8
#define	SIZE_16		16
#define	SIZE_32		32
#define	SIZE_64		64
#define	SIZE_128	128
#define	SIZE_256	256
#define	SIZE_512	512
#define	SIZE_1024	1024
#define	SIZE_2048	2048

/* Enums */

/* Structures */

/* Function Declaration */
int uPrintf(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /* __COMMON_H */

/****************************** (C) COPYRIGHT @ ICONT *****************************/
/* EOF */
