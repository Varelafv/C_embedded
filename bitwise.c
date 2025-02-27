 RCC->AHBENR |= ((1<<17) | (1<<19)) ;
  GPIOC->MODER &= ~((1<<26)| (1<<27));
  GPIOA->MODER &= ~(3U << (5 * 2));
  GPIOA->MODER |= (1U << ( 5* 2));
  while (1)
  {
	   SystickDelay(1000);
    	GPIOA->ODR ^= (1<<5);


  /* USER CODE END 3 */
} }


void SystickDelay (int ms ){
	  int i = 0 ;
	  SysTick->LOAD = 48000 - 1 ;
	  SysTick->VAL = 0; //reset the systick counter flag
	  SysTick->CTRL = ((1<<2) | (1<<0) | (1<<1)); //enable systick and select processor clock

	  for (int i =0 ; i<ms;i++){
		  while( (SysTick->CTRL & (1<<16)) == 0 ){

		  }

	  }

}