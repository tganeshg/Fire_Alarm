/**********************************************************************************
* @file           : main.c														  *
* @brief          : Main program body											  * 
***********************************************************************************
*		M	A	I	N	.	C			  										  *
***********************************************************************************
/--File Histroy ------------------------------------------------------------------/
*	Date			Version		Name	Description							      *
/---------------------------------------------------------------------------------/
*	25-Nov-2020		01.00		GAN		Initial Development.	  			  	  *
**********************************************************************************/

/* Includes */
#include "common.h"
#include "main.h"
#include "cmsis_os.h"
#include "lwip.h"

/* Local Macros */

/* Externs */

/* Global Variables */

/* Structure variables */
extern UART_HandleTypeDef 	pHuart;
ADC_HandleTypeDef 			hadc1;
DS1307_Handle 				ds1307;
DS1307_TIME 				curTime;
I2C_HandleTypeDef 			i2c_handle;

/* Definitions for defaultTask */
osThreadId_t ethernetTaskHandle;
const osThreadAttr_t ethernetTaskAttr = {
  .name = "ethernetTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

/*** Function  Definations ***/
//* Private *//
static void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the RCC Oscillators according to the specified parameters in the RCC_OscInitTypeDef structure. **/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.Prediv1Source = RCC_PREDIV1_SOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
	RCC_OscInitStruct.PLL2.PLL2State = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		ErrorHandler();
	}
	/** Initializes the CPU, AHB and APB buses clocks **/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
		ErrorHandler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		ErrorHandler();
	}
	/** Configure the Systick interrupt time **/
	__HAL_RCC_PLLI2S_ENABLE();
}

static void ADCInit(void)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	/** Common config  **/
	hadc1.Instance = ADC1;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		ErrorHandler();
	}
	/** Configure Regular Channel **/
	sConfig.Channel = ADC_CHANNEL_4;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		ErrorHandler();
	}
}

static void debugUartInit(void)
{
	pHuart.Instance = USART1;
	pHuart.Init.BaudRate = 115200;
	pHuart.Init.WordLength = UART_WORDLENGTH_8B;
	pHuart.Init.StopBits = UART_STOPBITS_1;
	pHuart.Init.Parity = UART_PARITY_NONE;
	pHuart.Init.Mode = UART_MODE_TX_RX;
	pHuart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	pHuart.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&pHuart) != HAL_OK)
	{
		ErrorHandler();
	}
}

static void gpioClkInit(void)
{
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

}

//* Main *//
int main(void)
{
	/* HAL Init */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	gpioClkInit();
	debugUartInit();
	ADCInit();

	/* Welcome */
	uPrintf("!! Fire Alarm Version 1.00 !!\r\n");

	if (confDS1307() != DS1307_RES_OK)
  	{
		uPrintf("Time  DS1307 Init done !!\r\n");
  	}

	/* Init scheduler */
	osKernelInitialize();

	/* creation of defaultTask */
	ethernetTaskHandle = osThreadNew(ethernetTask, NULL, &ethernetTaskAttr);

	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */
	/* Infinite loop */
	while (1)
	{
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 2);
		uint32_t AD_RES = HAL_ADC_GetValue(&hadc1);
		uPrintf("ADC Resolution = %hu\r\n",AD_RES);
		HAL_Delay(1000);

		curTime = DS1307_GetTime(&ds1307);
		uPrintf("\r\n time: %d:%d:%d | %d/%d/%d\r\n",curTime.hour,
														 curTime.minute,
														 curTime.second,
														 curTime.date,
														 curTime.month,
														 curTime.year);
		HAL_Delay(1000);
	}
}

//* Public *//
DS1307_RESULT confDS1307(void)
{
	ds1307.i2c = &i2c_handle;
	ds1307.DS1307_ADDRESS = 0xD0;
	ds1307.DS1307_CLOCK = 100000;
	ds1307.DS1307_I2Cx = I2C1;
	ds1307.TIMEOUT = 1000;
	DS1307_RESULT res;

	res = DS1307_Init(&ds1307);
	if(res != DS1307_RES_OK)
	{
		return DS1307_RES_ERROR;
	}

	res = DS1307_ClockResume(&ds1307);
	if(res != DS1307_RES_OK)
	{
		return DS1307_RES_ERROR;
	}

	res = DS1307_EnableSquareWave(&ds1307);
	if(res != DS1307_RES_OK)
	{
		return DS1307_RES_ERROR;
	}

	res = DS1307_SelectRate(&ds1307, DS1307_RATE_1HZ);
	if(res != DS1307_RES_OK)
	{
		return DS1307_RES_ERROR;
	}

	return DS1307_RES_OK;
}

void set_time(void)
{
	curTime.hour=15;
	curTime.minute=03;
	curTime.second=00;
	curTime.date=24;
	curTime.month=11;
	curTime.year=2020;
	DS1307_SetTime(&ds1307,curTime);
}

void ethernetTask(void *argument)
{
	/* init code for LWIP */
	MX_LWIP_Init();

	/* Infinite loop */
	for(;;)
	{
		osDelay(1);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2) 
	{
		HAL_IncTick();
	}
}

void ErrorHandler(void)
{
  __disable_irq();
  while(1);
}

/************************ (C) COPYRIGHT ICONT *****END OF FILE****/
