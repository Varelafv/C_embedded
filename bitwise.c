
  GPIOA->MODER &= ~(3U << (5 * 2));
  GPIOA->MODER |= (1U << ( 5* 2));
  send ('a');

  while (1)
  {

  /* USER CODE END 3 */
}

}
void send (char data){

	USART2->DR = data ;
	while (!(USART2->ISR & (1<<6))){}

}
void UART_init_var ( void ){

	  USART2->CR1 |= (1<<0) ; // USART enable
	  USART2->CR1 &= ~((1<<28) | (1<<12)); // length word
	  USART2->CR2 &=~ ((1<<13) | (1<<12));  // n of stop bit
	  USART2->BRR = 0x1388;  // USART2->BRR =(312 << 4) | 8;
	  USART2->CR1 |= (1<<3);
	  //USART2->DR  = data ;  // writing the data in the DR register
	  RCC->APB1ENR |= (1<<17);


}
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
/**