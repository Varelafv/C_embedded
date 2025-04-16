/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
//static void MX_USART2_UART_Init(void);
void send (char data);
void UART_init_var ( void );
void SystickDelay (int ms );
void recv ();
void GPIO_init (void);
void ADC_init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
char  data ;
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
 // HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
 // MX_GPIO_Init();
   //MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  UART_init_var();
  GPIO_init();
  send ('a');
  ADC_init();
  uint16_t result =0 ;
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
          /*
	  SystickDelay(500);
	  recv();
	  send (data);
	  GPIOA->ODR ^=(1<<5);
	  if(data == 'b'){
		  GPIOA->ODR ^=(1<<0);

	  }*/
	  send ('a');
	  SystickDelay(500);
	  ADC1->CR |=(1<<2); //starting
	  while(!(ADC1->ISR & (1<<2))); // wait the conversion finished
	  result = ADC1->DR;
      send(result);
      printf("value ",result);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}


void send (char data){
    while (!(USART2->ISR & (1 << 7))); // Attendre TXE (buffer vide)
    USART2->TDR = data;
    while (!(USART2->ISR & (1 << 6))); // Attendre TC (transmission complète)
}
void recv (){

	while (!(USART2->ISR & (1<<5)));  // WAIT  CHARACTER ARRIVES
	data = USART2->RDR ;// writting the data in data .
}
void GPIO_init (void){
	   RCC->AHBENR  |= (1 << 17); // GPIOA clock enable
	    GPIOA->MODER &= ~ ((3U << (5 * 2)) | (3U << (0 * 2))) ;
	    GPIOA->MODER |= (1U << ( 5* 2)) | (3U << ( 0* 2)) ;  // ANALOGUE MODE
}
void ADC_init(void){
	RCC->APB2ENR |=(1<<9);
   /// divide 48MHz for 12MHZ
	ADC1->CFGR2 |=(1<<31);
	ADC1->CFGR2 &=~(1<<30);
	ADC1->CR |=(1<<0); // set ADC on
	while(!(ADC1->ISR & (1<<0))); //waiting ADC be ready
	ADC1->CHSELR|=(1<<0); //selectioner channel 1

}
void UART_init_var(void) {
    // 1. Activer l'horloge pour USART2 et GPIOA
    RCC->APB1ENR |= (1 << 17); // USART2 clock enable
    //led trateement
    RCC->AHBENR  |= (1 << 17); // GPIOA clock enable




    // 2. Configurer PA2 en Alternate Function (AF7 pour USART2_TX)
    GPIOA->MODER  &= ~ ((3U << (2 * 2)) |(3U << (3 * 2)) ); // Effacer les bits de PA2
    GPIOA->MODER  |=   (2U << (2 * 2)) | (2U << (2 * 3) )  ; // Mettre PA2 et PA3 en Alternate Function

    GPIOA->OTYPER &= ~(1U << 2);       // Sortie push-pull
    GPIOA->PUPDR  &= ~((3U << (2 * 2)) | (3U << (3 * 2))); // Désactiver les pull-up/pull-down

    GPIOA->AFR[0]=  (1U << (4 * 2)); // PA2 en AF1 (USART2_TX)
    GPIOA->AFR[0]=  (1U << (4 * 3)); // PA3 en AF1 (USART2_TX)


    // 3. Désactiver USART2 avant configuration
    USART2->CR1 &= ~(1U << 0);  // USART disable

    // 4. Configurer le baud rate
    USART2->BRR = 0x1388; // 48 MHz / 9600 bauds

    // 5. Configurer la trame (8 bits, 1 stop bit, pas de parité)
    USART2->CR1 &= ~((1U << 28) | (1U << 12)); // 8 bits de données
    USART2->CR2 &= ~((1U << 13) | (1U << 12)); // 1 bit de stop

    // 6. Activer l’émetteur et le récepteur
    USART2->CR1 |= (1U << 3); // TX enable
    USART2->CR1 |= (1U << 2); // RX enable

    // 7. Activer USART2
    USART2->CR1 |= (1U << 0); // USART enable
    // Vérifier que l'USART est bien activé

}

void SystickDelay (int ms ){
	  int i = 0 ;
	  SysTick->LOAD = 48000 - 1 ;
	  SysTick->VAL = 0; //reset the systick counter flag
	  SysTick->CTRL = ((1<<2) | (1<<0) | (1<<1)); //enable systick and select processor clock

	  for (i =0 ; i<ms;i++){
		  while( (SysTick->CTRL & (1<<16)) == 0 ){

		  }

	  }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
