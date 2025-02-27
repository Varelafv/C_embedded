  RCC->AHBENR |= ((1<<17) | (1<<19)) ;
  RCC->APB1ENR  |= (1<<0); // ananble clock
  GPIOC->MODER &= ~((1<<26)| (1<<27));
  GPIOA->MODER &= ~(3U << (5 * 2));
  GPIOA->MODER |= (1U << ( 5* 2));

  TIM2->ARR  = 10000 -1 ; // reaload = 10000 -1
  TIM2->PSC  = 4800 -1;  // divided by 4800
  TIM2->CNT  = 0 ;   // clear timer counter
  TIM2->CR1  |=(1<<0) ; // time start .
  while (1)
  {
	   //SystickDelay(1000);
	  if(TIM2->SR & (1<<0)){  // WAIT uif + 1
		  TIM2->SR &=~(1<<0); //clear flag
    	  GPIOA->ODR ^= (1<<5);

	  }
  /* USER CODE END 3 */
} }
