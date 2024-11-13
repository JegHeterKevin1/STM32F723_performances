/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define TAB_SIZE 30000
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */



int comparator ( const void * first, const void * second ) {
	uint32_t firstInt = * (const uint32_t *) first;
	uint32_t secondInt = * (const uint32_t *) second;
	return firstInt - secondInt;
}

static uint32_t qsort_and_gettime(char ext_RAM){

	uint32_t timeTaken = 0;
	uint32_t timeBefore = HAL_GetTick();
	for(int j=0; j<5; j++){

		/*TEST DES PERFORMANCES AVEC QSORT*/
		if(ext_RAM){

			uint32_t *int_tab = (uint32_t*) 0x60000000;
			for(int i=TAB_SIZE; i>0; i--) {
				int_tab[TAB_SIZE-i] = i;
			}
			qsort(int_tab, TAB_SIZE, sizeof(uint32_t), comparator);
			uint32_t timeAfter = HAL_GetTick();
			timeTaken = timeTaken + timeAfter - timeBefore;

		}
		else{

			uint32_t int_tab[TAB_SIZE];
			for(int i=TAB_SIZE; i>0; i--){
				int_tab[TAB_SIZE-i] = i;
			}
			qsort(int_tab, TAB_SIZE, sizeof(uint32_t), comparator);
			uint32_t timeAfter = HAL_GetTick();
			timeTaken = timeTaken + timeAfter - timeBefore;

		}





	}
	return timeTaken;

}

static void displayTime(uint32_t timeTaken, uint16_t line){

	char resultTime[256];
	itoa( timeTaken , resultTime ,10);
	char FinalSentence[1024] = "Temps pris : ";
	strcat(FinalSentence, resultTime);
	strcat(FinalSentence, " ms");
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_DisplayStringAtLine(line, (uint8_t*) FinalSentence);
}



/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MPU Configuration--------------------------------------------------------*/
	MPU_Config();

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Enable the cache */
	SCB_EnableDCache();
	SCB_EnableICache();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	/* USER CODE BEGIN 2 */

	if(BSP_LCD_Init() != LCD_OK) return -1;
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_Clear(LCD_COLOR_WHITE);

	char str[20] = "Leo le GOAT";
	BSP_LCD_DisplayStringAtLine(0, (uint8_t*) str);

	//  uint16_t color = 0x0000;
	//  for(uint16_t y = 0; y<240; y++){
	//	  for(uint16_t x=0; x<240; x++){
	//		  BSP_LCD_DrawPixel(x, y, color);
	//		  color = color+10;
	//	  }
	//  }

	displayTime(qsort_and_gettime(0), 10);
	/*Disable the I cache but stay on RAM*/
	SCB_DisableICache();
	displayTime(qsort_and_gettime(0), 11);
	/* Disable the D cache but stay on RAM*/
	SCB_DisableDCache();
	displayTime(qsort_and_gettime(0), 12);
	/* Enable the I cache but not D cache and stay on RAM */
	SCB_EnableICache();
	displayTime(qsort_and_gettime(0), 13);






	/* Using the extern SRAM to see performances */
	SCB_EnableDCache(); // D cache & I cache are on
	displayTime(qsort_and_gettime(1), 14);
	/*Disable the I cache but is on ext RAM*/
	SCB_DisableICache();
	displayTime(qsort_and_gettime(1), 15);
	/* Disable the D cache but is on ext RAM*/
	SCB_DisableDCache();
	displayTime(qsort_and_gettime(1), 16);
	/* Enable the I cache but not D cache and is on ext RAM */
	SCB_EnableICache();
	displayTime(qsort_and_gettime(1), 17);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}


/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 216;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Activate the Over-Drive mode
	 */
	if (HAL_PWREx_EnableOverDrive() != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOE, ARD_D7_GPIO_Pin|ARD_D8_GPIO_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOG, WIFI_RST_Pin|WIFI_GPIO_0_Pin|PMOD_GPIO_0_Pin|USB_OTGFS_PPWR_EN_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD, WIFI_GPIO_2_Pin|WIFI_CH_PD_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, STMOD_UART4_RXD_s_Pin|ARD_D2_GPIO_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOI, PMOD_SPI2_MOSI_Pin|PMOD_SPI2_MISO_Pin|GPIO_PIN_10, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(PMOD_SEL_0_GPIO_Port, PMOD_SEL_0_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, USB_OTG_FS_ID_Pin|SYS_LD_USER1_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOH, PMOD_GPIO_1_Pin|ARD_D4_GPIO_Pin|USB_OTGHS_PPWR_EN_Pin|CTP_RST_Pin
			|LCD_RST_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, USB_OTG_HS_ID_Pin|SYS_LD_USER2_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : ARD_D7_GPIO_Pin ARD_D8_GPIO_Pin */
	GPIO_InitStruct.Pin = ARD_D7_GPIO_Pin|ARD_D8_GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pin : QSPI_D2_Pin */
	GPIO_InitStruct.Pin = QSPI_D2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
	HAL_GPIO_Init(QSPI_D2_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : PSRAM_NBL1_Pin PSRAM_NBL0_Pin LCD_PSRAM_D10_Pin LCD_PSRAM_D5_Pin
                           LCD_PSRAM_D6_Pin LCD_PSRAM_D8_Pin LCD_PSRAM_D11_Pin LCD_PSRAM_D4_Pin
                           LCD_PSRAM_D7_Pin LCD_PSRAM_D9_Pin LCD_PSRAM_D12_Pin */
	GPIO_InitStruct.Pin = PSRAM_NBL1_Pin|PSRAM_NBL0_Pin|LCD_PSRAM_D10_Pin|LCD_PSRAM_D5_Pin
			|LCD_PSRAM_D6_Pin|LCD_PSRAM_D8_Pin|LCD_PSRAM_D11_Pin|LCD_PSRAM_D4_Pin
			|LCD_PSRAM_D7_Pin|LCD_PSRAM_D9_Pin|LCD_PSRAM_D12_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pins : SAI2_I2C1_SCL_Pin SAI2_I2C1_SDA_Pin */
	GPIO_InitStruct.Pin = SAI2_I2C1_SCL_Pin|SAI2_I2C1_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : ARD_D11_TIM3_CH2_SPI1_MOSI_Pin ARD_D12_SPI1_MISO_Pin */
	GPIO_InitStruct.Pin = ARD_D11_TIM3_CH2_SPI1_MOSI_Pin|ARD_D12_SPI1_MISO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : WIFI_RST_Pin WIFI_GPIO_0_Pin PMOD_GPIO_0_Pin USB_OTGFS_PPWR_EN_Pin */
	GPIO_InitStruct.Pin = WIFI_RST_Pin|WIFI_GPIO_0_Pin|PMOD_GPIO_0_Pin|USB_OTGFS_PPWR_EN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	/*Configure GPIO pins : PSRAM_NE1_Pin LCD_PSRAM_D2_Pin LCD_PSRAM_NWE_Pin LCD_PSRAM_D3_Pin
                           LCD_PSRAM_NWED4_Pin LCD_PSRAM_D1_Pin LCD_PSRAM_D0_Pin PSRAM_A17_Pin
                           PSRAM_A16_Pin LCD_PSRAM_D15_Pin LCD_PSRAM_D14_Pin LCD_PSRAM_D13_Pin */
	GPIO_InitStruct.Pin = PSRAM_NE1_Pin|LCD_PSRAM_D2_Pin|LCD_PSRAM_NWE_Pin|LCD_PSRAM_D3_Pin
			|LCD_PSRAM_NWED4_Pin|LCD_PSRAM_D1_Pin|LCD_PSRAM_D0_Pin|PSRAM_A17_Pin
			|PSRAM_A16_Pin|LCD_PSRAM_D15_Pin|LCD_PSRAM_D14_Pin|LCD_PSRAM_D13_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pin : UART_TXD_WIFI_RX_Pin */
	GPIO_InitStruct.Pin = UART_TXD_WIFI_RX_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
	HAL_GPIO_Init(UART_TXD_WIFI_RX_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : STMOD_TIM2_CH1_2_ETR_Pin ARD_D10_TIM2_CH2_SPI1_NSS_Pin */
	GPIO_InitStruct.Pin = STMOD_TIM2_CH1_2_ETR_Pin|ARD_D10_TIM2_CH2_SPI1_NSS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : ARD_D3_TIM9_CH1_Pin ARD_D6_TIM9_CH2_Pin */
	GPIO_InitStruct.Pin = ARD_D3_TIM9_CH1_Pin|ARD_D6_TIM9_CH2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF3_TIM9;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pin : NC1_Pin */
	GPIO_InitStruct.Pin = NC1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(NC1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : QSPI_NCS_Pin */
	GPIO_InitStruct.Pin = QSPI_NCS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
	HAL_GPIO_Init(QSPI_NCS_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : SAI2_INT_Pin */
	GPIO_InitStruct.Pin = SAI2_INT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(SAI2_INT_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : SAI2_SD_B_Pin */
	GPIO_InitStruct.Pin = SAI2_SD_B_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF10_SAI2;
	HAL_GPIO_Init(SAI2_SD_B_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : WIFI_GPIO_2_Pin WIFI_CH_PD_Pin */
	GPIO_InitStruct.Pin = WIFI_GPIO_2_Pin|WIFI_CH_PD_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pins : STMOD_UART4_RXD_s_Pin ARD_D2_GPIO_Pin */
	GPIO_InitStruct.Pin = STMOD_UART4_RXD_s_Pin|ARD_D2_GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : QSPI_D1_Pin QSPI_D0_Pin */
	GPIO_InitStruct.Pin = QSPI_D1_Pin|QSPI_D0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : PA12 PA11 */
	GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : SAI2_FS_A_Pin SAI2_SD_A_Pin SAI2_SCK_A_Pin SAI2_MCLK_A_Pin */
	GPIO_InitStruct.Pin = SAI2_FS_A_Pin|SAI2_SD_A_Pin|SAI2_SCK_A_Pin|SAI2_MCLK_A_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF10_SAI2;
	HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

	/*Configure GPIO pins : LCD_NE_Pin PSRAM_A15_Pin PSRAM_A14_Pin PSRAM_A13_Pin
                           PSRAM_A12_Pin PSRAM_A11_Pin PSRAM_A10_Pin */
	GPIO_InitStruct.Pin = LCD_NE_Pin|PSRAM_A15_Pin|PSRAM_A14_Pin|PSRAM_A13_Pin
			|PSRAM_A12_Pin|PSRAM_A11_Pin|PSRAM_A10_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	/*Configure GPIO pins : PMOD_SPI2_MOSI_Pin PMOD_SPI2_MISO_Pin PI10 */
	GPIO_InitStruct.Pin = PMOD_SPI2_MOSI_Pin|PMOD_SPI2_MISO_Pin|GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

	/*Configure GPIO pin : CTP_INT_Pin */
	GPIO_InitStruct.Pin = CTP_INT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(CTP_INT_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : UART_RXD_WIFI_TX_Pin */
	GPIO_InitStruct.Pin = UART_RXD_WIFI_TX_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
	HAL_GPIO_Init(UART_RXD_WIFI_TX_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : PMOD_SEL_0_Pin PMOD_GPIO_1_Pin ARD_D4_GPIO_Pin USB_OTGHS_PPWR_EN_Pin
                           CTP_RST_Pin LCD_RST_Pin */
	GPIO_InitStruct.Pin = PMOD_SEL_0_Pin|PMOD_GPIO_1_Pin|ARD_D4_GPIO_Pin|USB_OTGHS_PPWR_EN_Pin
			|CTP_RST_Pin|LCD_RST_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

	/*Configure GPIO pins : PMOD_SPI2_SCK_Pin PMOD_SPI2_NSS_Pin */
	GPIO_InitStruct.Pin = PMOD_SPI2_SCK_Pin|PMOD_SPI2_NSS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

	/*Configure GPIO pins : USB_OTG_FS_ID_Pin SYS_LD_USER1_Pin */
	GPIO_InitStruct.Pin = USB_OTG_FS_ID_Pin|SYS_LD_USER1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PSRAM_A0_Pin PSRAM_A2_Pin PSRAM_A1_Pin PSRAM_A3_Pin
                           PSRAM_A4_Pin PSRAM_A5_Pin PSRAM_A7_Pin PSRAM_A6_Pin
                           PSRAM_A9_Pin PSRAM_A8_Pin */
	GPIO_InitStruct.Pin = PSRAM_A0_Pin|PSRAM_A2_Pin|PSRAM_A1_Pin|PSRAM_A3_Pin
			|PSRAM_A4_Pin|PSRAM_A5_Pin|PSRAM_A7_Pin|PSRAM_A6_Pin
			|PSRAM_A9_Pin|PSRAM_A8_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	/*Configure GPIO pins : STMOD_UART4_TXD_Pin STMOD_UART4_RXD_Pin */
	GPIO_InitStruct.Pin = STMOD_UART4_TXD_Pin|STMOD_UART4_RXD_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

	/*Configure GPIO pin : PA9 */
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : CTP_SCL_Pin */
	GPIO_InitStruct.Pin = CTP_SCL_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
	HAL_GPIO_Init(CTP_SCL_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : LCD_TE_INT_Pin */
	GPIO_InitStruct.Pin = LCD_TE_INT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(LCD_TE_INT_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : VCP_RX_Pin VCP_TX_Pin */
	GPIO_InitStruct.Pin = VCP_RX_Pin|VCP_TX_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : ARD_D15_STMOD_I2C2_SCL_Pin ARD_D14_STMOD_I2C2_SDA_Pin */
	GPIO_InitStruct.Pin = ARD_D15_STMOD_I2C2_SCL_Pin|ARD_D14_STMOD_I2C2_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

	/*Configure GPIO pins : PMOD_UART7_TXD_Pin PMOD_UART7_RXD_Pin PMOD_UART7_CTS_Pin PMOD_UART7_RTS_Pin */
	GPIO_InitStruct.Pin = PMOD_UART7_TXD_Pin|PMOD_UART7_RXD_Pin|PMOD_UART7_CTS_Pin|PMOD_UART7_RTS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF8_UART7;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	/*Configure GPIO pin : ARD_A3_ADC3_IN8_Pin */
	GPIO_InitStruct.Pin = ARD_A3_ADC3_IN8_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(ARD_A3_ADC3_IN8_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : LCD_BL_Pin */
	GPIO_InitStruct.Pin = LCD_BL_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
	HAL_GPIO_Init(LCD_BL_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : USB_OTGHS_OVCR_INT_Pin */
	GPIO_InitStruct.Pin = USB_OTGHS_OVCR_INT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(USB_OTGHS_OVCR_INT_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : ARD_A4_Pin ARD_A5_Pin ARD_A2_Pin */
	GPIO_InitStruct.Pin = ARD_A4_Pin|ARD_A5_Pin|ARD_A2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : STMOD_SPI2_MISOs_Pin STMOD_SPI2_MOSIs_Pin */
	GPIO_InitStruct.Pin = STMOD_SPI2_MISOs_Pin|STMOD_SPI2_MOSIs_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin : QSPI_CLK_Pin */
	GPIO_InitStruct.Pin = QSPI_CLK_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
	HAL_GPIO_Init(QSPI_CLK_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : ARD_D9_TIM12_CH1_Pin */
	GPIO_InitStruct.Pin = ARD_D9_TIM12_CH1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF9_TIM12;
	HAL_GPIO_Init(ARD_D9_TIM12_CH1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : CTP_SDA_Pin */
	GPIO_InitStruct.Pin = CTP_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
	HAL_GPIO_Init(CTP_SDA_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : QSPI_D3_Pin */
	GPIO_InitStruct.Pin = QSPI_D3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
	HAL_GPIO_Init(QSPI_D3_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : ARD_A1_Pin ARD_A0_Pin */
	GPIO_InitStruct.Pin = ARD_A1_Pin|ARD_A0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : ARD_D1_USART2_TX_Pin ARD_D0_USART2_RX_Pin */
	GPIO_InitStruct.Pin = ARD_D1_USART2_TX_Pin|ARD_D0_USART2_RX_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : ARD_D13_SPI1_SCK_Pin */
	GPIO_InitStruct.Pin = ARD_D13_SPI1_SCK_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
	HAL_GPIO_Init(ARD_D13_SPI1_SCK_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : USB_OTG_HS_ID_Pin SYS_LD_USER2_Pin */
	GPIO_InitStruct.Pin = USB_OTG_HS_ID_Pin|SYS_LD_USER2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : USB_OTG_HS_VBUS_Pin USB_OTGFS_OVCR_INT_Pin PMOD_INT_Pin */
	GPIO_InitStruct.Pin = USB_OTG_HS_VBUS_Pin|USB_OTGFS_OVCR_INT_Pin|PMOD_INT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : ARD_D5_STMOD_TIM3_CH3_Pin */
	GPIO_InitStruct.Pin = ARD_D5_STMOD_TIM3_CH3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(ARD_D5_STMOD_TIM3_CH3_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : PMOD_RESET_Pin */
	GPIO_InitStruct.Pin = PMOD_RESET_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(PMOD_RESET_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : PB14 PB15 */
	GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_OTG_HS_FS;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* MPU Configuration */

void MPU_Config(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct = {0};

	/* Disables the MPU */
	HAL_MPU_Disable();

	/** Initializes and configures the Region and the memory to be protected
	 */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER0;
	MPU_InitStruct.BaseAddress = 0x64000000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
	MPU_InitStruct.SubRegionDisable = 0x0;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);
	/* Enables the MPU */
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
