/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#define MILLER_RABIN_MAX_ITERATION 256
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */


//#define IDLE_MODE
//#define IDLE_IT_MODE
//#define SLEEP_MODE
#define STOP_MODE


/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
RNG_HandleTypeDef RngHandle;

uint32_t cntr = 0;
volatile uint32_t pNumber = 0;

char *data = NULL; // pointer used to display value on a screen

/* Used for storing 128 Random 32bit Numbers */
uint32_t aRandom32bit[MILLER_RABIN_MAX_ITERATION];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MPU_Config(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
  * @brief  Configures system clock after wake-up from STOP: enable HSE, PLL
  *         and select PLL as system clock source.
  * @param  None
  * @retval None
  */
static void SYSCLKConfig_WAKE(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  uint32_t pFLatency = 0;

  /* Get the Oscillators configuration according to the internal RCC registers */
  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

  /* After wake-up from STOP reconfigure the system clock: Enable HSE and PLL */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Get the Clocks configuration according to the internal RCC registers */
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &pFLatency);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, pFLatency) != HAL_OK)
  {
    Error_Handler();
  }
}

//Generate n rand number inside table
void generateRandNumber(uint32_t n, uint32_t *table){
 uint32_t counter;

 for (counter = 0; counter < n; counter++)
 {
   if (HAL_RNG_GenerateRandomNumber(&RngHandle, &table[counter]) != HAL_OK)
   {
     /* Random number generation error */
     Error_Handler();
   }
 }
}

/* RussianPeasant to multiply large number without overflow */
static uint32_t RussianPeasant(uint32_t a, uint32_t b, uint32_t c)
    {
        const uint32_t overflow_limit = 1L << 16;

        if (a < overflow_limit && b < overflow_limit)
            return (a * b % c); // No possibility of overflow.
        if (c < overflow_limit)
            return (a % c) * (b % c) % c;
        uint32_t ret = 0;
        while(b != 0) {
            if((b&1L) != 0L) {
                ret += a;
                ret %= c;
            }
            a *= 2;
            a %= c;
            b = b >> 1;
        }
        return ret;
    }

/* Iterative Function to calculate (x^y)%p in O(log y) */
uint32_t power(uint32_t x, uint32_t y, uint32_t p)
{
    int res = 1;      // Initialize result

    x = x % p;  // Update x if it is more than or equal to p

    if (x == 0) return 0; // In case x is divisible by p;

    while (y > 0)
    {
        // If y is odd, multiply x with result
        if (y & 1)
            res = RussianPeasant(res,x,p);//((res%p)*(x%p)) % p;

        // y must be even now
        y = y>>1; // y = y/2
        x = RussianPeasant(x,x,p);//((x%p)*(x%p)) % p;
    }
    return res;
}

/* checks if number num is prime, the more iterations you add the safer the result will be
 *  return 0 if the number is prime -1 otherwise */
int isPrime(uint32_t num, uint32_t iteration)
{
  uint32_t r = num - 1;
  uint32_t s = 0;
  uint32_t a,x;
  uint32_t i,j;

  if(num == 2 || num == 3){
    return 0;
  }
  if(num == 0 || num == 1 || (num % 2) == 0){
    return -1;
  }

  //find r and s
  while(r%2 == 0){
    r = r >> 1;
    s++;
  }

  //Miller–Rabin test
  for(i=0;i<iteration;i++){
    a = (aRandom32bit[i]%(num-4))+2; //correspond to a rand number betwhen 4,num-2
    x = power(a, r, num); // a^r mod num

    if (x != 1  && x != num-1){
      j=1;
      while(j<s && x!= (num-1)){
        x=RussianPeasant(x,x,num);
        if(x == 1)
          return -1;
        j++;
      }
      if(x != (num-1))
        return -1;
    }
  }
 return 0;
}

uint32_t findNextPrime(uint32_t n)
{
  if(n%2 == 0)
    n++;
  while(isPrime(n,MILLER_RABIN_MAX_ITERATION)){
    n+=2;
  }
  return n;
}
/* USER CODE END 0 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){


#if defined(SLEEP_MODE) || defined(STOP_MODE)
	  HAL_ResumeTick();
#if defined(STOP_MODE)
	  SYSCLKConfig_WAKE();
#endif
#endif

	  if(GPIO_Pin == USER_BUTTON_PIN){
		  pNumber = findNextPrime(cntr & 0x7FFFFFFF);
		  cntr = pNumber+1;
		  snprintf(data,15,"%lu", pNumber);
		  BSP_LCD_DisplayStringAtLine(4, (uint8_t*) data);
	  }
}


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  MPU_Config();

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  data = malloc(16*sizeof(char)); //used for display purpose

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
#if defined(SLEEP_MODE) || defined(STOP_MODE)
	__HAL_RCC_PWR_CLK_ENABLE();
#endif

  /* LCD Init */
  BSP_LCD_Init();
  /* Clear the Background Layer */
  BSP_LCD_Clear(LCD_COLOR_BLACK);
  /* Set the Text */
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  /* Set the Back Color */
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK);


  /*## Configure the RNG peripheral */
  RngHandle.Instance = RNG;

  /* DeInitialize the RNG peripheral */
  if (HAL_RNG_DeInit(&RngHandle) != HAL_OK)
  {
    /* DeInitialization Error */
    Error_Handler();
  }

  /* Initialize the RNG peripheral */
  if (HAL_RNG_Init(&RngHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* genrate a set of rand number needed by the Miller Rabin test */
  generateRandNumber(MILLER_RABIN_MAX_ITERATION,aRandom32bit);

  /* USER CODE BEGIN SysInit */
  /* Configure USER Button */
#if (defined(SLEEP_MODE) || defined(IDLE_IT_MODE) || defined(STOP_MODE)) && !defined(IDLE_MODE)
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
#endif

#if defined (IDLE_MODE) && !defined(SLEEP_MODE) && !defined(STOP_MODE) && !defined(IDLE_IT_MODE)
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
#endif
  /* USER CODE END SysInit */

  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
#if defined (IDLE_MODE) && !defined(SLEEP_MODE) && !defined(STOP_MODE) && !defined(IDLE_IT_MODE)

    if(BSP_PB_GetState(BUTTON_USER) != RESET){
      pNumber = findNextPrime(cntr & 0x7FFFFFFF);
      cntr = pNumber;
      snprintf(data, 15, "%lu", pNumber);
      BSP_LCD_DisplayStringAtLine(4, (uint8_t*)data);
    }

#endif

#if !defined(SLEEP_MODE) && !defined(STOP_MODE)
    cntr++;
    HAL_Delay(100);
#endif

#if defined(SLEEP_MODE)
    HAL_SuspendTick();
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
#endif

#if defined(STOP_MODE)
    HAL_SuspendTick();
    HAL_PWREx_EnableFlashPowerDown();
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
#endif

    /* USER CODE BEGIN 3 */
  }
  free(data);
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
