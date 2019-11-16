//Kenya Andrews
#include<stdio.h>
#include<stdlib.h>
#include "STM32L1xx.h" /* Microcontroller information */

int keyPressed = 0;
int intr = 0;
int t = 0;
int dr = 0;
int timer11ccr1 = 0;
int avgdr = 0;
int countdr = 0;
int totdr = 0;
int amparray[600];
int count = 0;
int decitime = 0;
int startstop = 0;
int expectARR[10] = {46, 116, 186, 256, 326, 397, 467, 537, 607, 677}; //for 10 speeds these are the expected output values

/*******short delay function - do nothing*******/
void smalldelay() {
    for (int i=0; i<=800; i++) { //short delay loop
        } //do nothing
}

/************pinsetup stuff*********/
void pinSetup()
{
	
/* Configure PA1-PA0 as input to hold switch values*/
    RCC->AHBENR |= 0x01; /* Enable GPIOA clock (bit 0) */
   // GPIOA->MODER &= ~(0x0000003C); /* General purpose input mode */
		GPIOA->MODER &= ~(0x0000303C); /* clear PA6,PA1,PA0 */
	
	/*NVIC for PA1*/
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_ClearPendingIRQ(EXTI1_IRQn);
	
	/*SYSCFG module for PA1*/
	SYSCFG -> EXTICR[0] &= ~(0x000000F0);
	SYSCFG -> EXTICR[0] |= (0x00000002);
	
	/*EXTI module for PA1*/
	EXTI->FTSR |= 0x0002;
	EXTI->IMR = 0x0002;
	EXTI->PR |= 0x0002;
	
	/*configure PA7 and PA6 as alternate functions and PA5 as analog*/
	GPIOA->MODER |= 0x00002000; //PA6 alternate function
	GPIOA->MODER |= 0x00008000; //PA7 alternate function
	GPIOA->MODER |= 0x00000C00; //PA5 analog function
	GPIOA->AFR[0] &= ~(0xFF000000); //clear AFRL7 and AFRL6
	GPIOA->AFR[0] |= 0x33000000; //PA7 = pin3 of AFR3 and PA6 = pin3 of AFR3
  
	/************* Pull-Up/Pull-Down Stuff for A ***********/
		GPIOA->PUPDR &= ~0xFFFF;
		GPIOA->PUPDR |= 0x00004000; //PA7
	
  /****Configure PB7 -  PB4 as outputs and PB3 - PB0 as input pins to drive LEDs ****/
    RCC->AHBENR |= 0x02; /* Enable GPIOC clock (bit 2) */
    GPIOB->MODER &= ~(0x0000FFFF); /* Clear PC3-PC0 mode bits */	
		GPIOB->MODER |= (0x00005500); /* General purpose output mode*/
	
	/************* Pull-Up/Pull-Down Stuff for B ***********/
		GPIOB->PUPDR &= ~0xFFFF;
		GPIOB->PUPDR |= 0x00000055;
	
	
  /****** Configure PC3 - PC0 as output pins to drive LEDs ******/
    RCC->AHBENR |= 0x04; /* Enable GPIOC clock (bit 2) */
    GPIOC->MODER &= ~(0x0000FFFF); /* Clear PC3-PC0 mode bits */
    GPIOC->MODER |= (0x00005555); /* General purpose output mode*/
	
	/*************timer 10 setup***********/
	/*Initialize TIM10*/
	
	RCC-> APB2ENR |= RCC_APB2ENR_TIM10EN;
	
	TIM10 -> CNT=0; //reset timer
	//Timer formula: (16K/16MHz) = .001s
	RCC->CR |= RCC_CR_HSION;
	while((RCC->CR & RCC_CR_HSIRDY) == 0);
	RCC->CFGR |= RCC_CFGR_SW_HSI;
	RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
	//1k psc = 1599
	//100 psc = 15999
	
	TIM10->PSC = 1599;
	TIM10->ARR = 9;
	
	
	//TIM10->PSC = 0x0FF;
	TIM10->DIER |= TIM_DIER_UIE;	
	NVIC_EnableIRQ(TIM10_IRQn);
	TIM10->CCMR1 = 0x60; //PWM mode 1 (active to inactive)
	TIM10->CCER = 0x01; //active OC1 as high
	
	/*************timer 11 setup***********/
	/*Initialize TIM11*/
	
	RCC-> APB2ENR |= RCC_APB2ENR_TIM11EN;

	TIM11->ARR = 0x1750;
	TIM11->PSC = 0x0FF;
	
	TIM11->DIER |= TIM_DIER_UIE;	
	NVIC_EnableIRQ(TIM11_IRQn);


/*************ADC set up***********/
	RCC-> APB2ENR |= RCC_APB2ENR_ADC1EN;
	ADC1->CR2 |= 0x01;
	ADC1->SR = 0x00;

	//while((ADC1->SQR5 & 0x100)==0);
	
	ADC1->SMPR3 &= ~ADC_SMPR3_SMP5; //Clear SMP8 bits*
	ADC1->SMPR3 |= 0x00000003; //SMP5 = 3 
	
	ADC1->SQR5 &= ~ADC_SQR5_SQ1;
	ADC1->SQR5 |= 0x05; 
	
	ADC1->CR2 |= ADC_CR2_SWSTART;
	
	
/***********set priorities***************/
	NVIC_SetPriority(EXTI1_IRQn, 0);
	NVIC_SetPriority(TIM11_IRQn, 1);
	NVIC_SetPriority(TIM10_IRQn, 2);
}

/*******Keypad button interrupt stuff******/
//see what speed was pressed on keypad
void EXTI1_IRQHandler()
{
	intr = 0; //reset interrupt
	int thisrow = 0;
	GPIOB -> ODR = 0x0000 << 4; //clear PB7 - PB4, set all rows to 0
	
	//short delay
	smalldelay();
	
	//assuming 16Mhz, 16MHz/1kHz = 16,000 for PWM Period
	//assuming 16Mhz, 16MHz/100Hz = 160,000 for PWM Period
	//column 1
	GPIOB -> ODR = 0xFFEF; //drive column 1 low
	smalldelay();
	thisrow = GPIOB->IDR & 0xF;
	
	//row1
	if (thisrow == 0x000E)
	{
		keyPressed = 1; //start/stop stopwatch key
		//TIM10->CCR1 = 1;//16000*.1=1600 or 160000*.1
		startstop ++; //will control if we need to start or stop the timer
		intr = 1;
	}
	
	//row2
	if (thisrow == 0x000D)
	{
		keyPressed = 4;
		TIM10->CCR1 = 3.7;
		intr = 1;
	}
	
	//row3
	if (thisrow == 0x000B)
	{
		keyPressed = 7;
		TIM10->CCR1 = 5.8;
		intr = 1;
	}
	
	//row4
	if (thisrow == 0x0007)
	{
		keyPressed = 14; //actually *
		intr = 1;
	}
	
	//column 2
	GPIOB -> ODR = 0xFFDF; //drive column 2 low
	smalldelay();
	thisrow = GPIOB->IDR & 0xF;
	
	//row1
	if (thisrow == 0x000E)
	{
		keyPressed = 2; //reset key
		//TIM10->CCR1 = 2;
		intr = 1;
	}
	
	//row2
	if (thisrow == 0x000D)
	{
		keyPressed = 5;
		TIM10->CCR1 = 4.4;
		intr = 1;
	}
	
	//row3
	if (thisrow == 0x000B)
	{
		keyPressed = 8;
		TIM10->CCR1 = 6.5;
		intr = 1;
	}
	
	//row4
	if (thisrow == 0x0007)
	{
		keyPressed = 0;
		TIM10->CCR1 = 0x0000;
		intr = 1;
	}
	
	
	//column 3
	GPIOB -> ODR = 0xFFBF; //drive column 3 low
	smalldelay();
	thisrow = GPIOB->IDR & 0xF;
	
	//row1
	if (thisrow == 0x000E)
	{
		keyPressed = 3;
		TIM10->CCR1 = 3;
		intr = 1;
	}
	
	//row2
	if (thisrow == 0x000D)
	{
		keyPressed = 6;
		TIM10->CCR1 = 5.1;
		intr = 1;
	}
	
	//row3
	if (thisrow == 0x000B)
	{
		keyPressed = 9;
		TIM10->CCR1 = 7.2;
		intr = 1;
	}
	
	//row4
	if (thisrow == 0x0007)
	{
		keyPressed = 15; //actually #
		intr = 1;
	}
	
	
	//column 4
	GPIOB -> ODR = 0xFF7F; //drive column 4 low
	smalldelay();
	thisrow = GPIOB->IDR & 0xF;
	
	//row1
	if (thisrow == 0x000E)
	{
		keyPressed = 10; //A
		TIM10->CCR1 = 7.9;
		intr = 1;
	}
	
	//row2
	if (thisrow == 0x000D)
	{
		keyPressed = 11; //B
		TIM10->CCR1 = 8.6;
		intr = 1;
	}
	
	//row3
	if (thisrow == 0x000B)
	{
		keyPressed = 12; //C
		TIM10->CCR1 = 9.3; 
		intr = 1;
	}
	
	//row4
	if (thisrow == 0x0007)
	{
		keyPressed = 13; //actually D
		intr = 1;
	}

		
	GPIOB -> ODR = 0x0000 << 4; //clear PB7 - PB4, set all rows to 0
	smalldelay();
	
	EXTI->PR |= 0x0002;
	NVIC_ClearPendingIRQ(EXTI1_IRQn); //clear  NVIC
}

//display the one's place of the counter in binary form on leds
void displayLEDs(int count)
{
	//set PC3:PC0 for all cases
    switch(count)
    {
			 case 0: GPIOC->ODR  &= 0xFFF0; 
					GPIOC->ODR  |= 0x0000; 								
					break;
				
			case 1: GPIOC->ODR  &= 0xFFF0; 
							GPIOC->ODR  |= 0x0001;
					break;
					
			case 2: GPIOC->ODR  &= 0xFFF0; 
							GPIOC->ODR  |= 0x0002;
					break;
			 
			case 3: GPIOC->ODR  &= 0xFFF0; 
							GPIOC->ODR  |= 0x0003;
					break;
					
			case 4: GPIOC->ODR  &= 0xFFF0;
							GPIOC->ODR  |= 0x0004;
					break;
					
			case 5: GPIOC->ODR  &= 0xFFF0; 
							GPIOC->ODR  |= 0x0005;
					break;
				 
			case 6: GPIOC->ODR  &= 0xFFF0;
							GPIOC->ODR  |= 0x0006;
					break;
				 
			case 7: GPIOC->ODR  &= 0xFFF0;
							GPIOC->ODR  |= 0x0007;
					break;
		 
			case 8: GPIOC->ODR  &= 0xFFF0;
							GPIOC->ODR  |= 0x0008;
					break;
			 
			case 9: GPIOC->ODR  &= 0xFFF0;
							GPIOC->ODR  |= 0x0009;
					break;
    }
}

//display the tenths' place of the counter in binary form on leds
void displayLEDs2(int decitime)
{	
	//set PC7:PC4 for all cases
    switch(decitime)
    {
			case 0: GPIOC->ODR  &= 0xFF0F; 
					GPIOC->ODR  |= 0x0000; 								
					break;

			case 1: GPIOC->ODR  &= 0xFF0F;								
							GPIOC->ODR  |= 0x0010;
					break;
		
			case 2: GPIOC->ODR  &= 0xFF0F; 
							GPIOC->ODR  |= 0x0020;
					break;
		
			case 3: GPIOC->ODR  &= 0xFF0F; 								
							GPIOC->ODR  |= 0x0030;
					break;
		
			case 4: GPIOC->ODR  &= 0xFF0F;
							GPIOC->ODR  |= 0x0040;
					break;
					
			case 5: GPIOC->ODR  &= 0xFF0F;
							GPIOC->ODR  |= 0x0050;
					break;
					
			case 6: GPIOC->ODR  &= 0xFF0F;								
							GPIOC->ODR  |= 0x0060;
					break;
					
			case 7: GPIOC->ODR  &= 0xFF0F;
							GPIOC->ODR  |= 0x0070;
					break;
					
			case 8: GPIOC->ODR  &= 0xFF0F;								
							GPIOC->ODR  |= 0x0080;
					break;
			
			case 9: GPIOC->ODR  &= 0xFF0F; 								
							GPIOC->ODR  |= 0x0090;
					break;
    }
}

/***********Count Things****************/
void alwayscountup()
{
	//counter1 always counts up
    count++; //always increment the first count
	
	//then reset counter after it counts to 9
		if(count ==	10)
		{
			count = 0;
		}
}


void TIM10_IRQHandler() //every .1 seconds
{		
		TIM10->CR1 |= 0x01;
	
		while(1)
		{
			
		}
		
		
		TIM10-> SR &= ~0x01;
		NVIC_ClearPendingIRQ(TIM10_IRQn);
		TIM10 -> CNT=0; //reset timer
}


void TIM11_IRQHandler() //every .1 seconds adjust speed if not as expected
{		
		TIM11->CR1 |= 0x01;
		ADC1->CR2 |= ADC_CR2_SWSTART;
		while((ADC1->SR & 0x02) == 0);
		dr = ADC1->DR;
    
		if(keyPressed == 0)
		{
					TIM10->CCR1 = 0;
		}
		
		else if((keyPressed > 2 &&  keyPressed < 13) && dr <= expectARR[keyPressed-3])
		{
			TIM10->CCR1 = TIM10->CCR1 + .1;
		}
		
		else if((keyPressed > 2 && keyPressed < 13) && dr > expectARR[keyPressed-3])
		{
			TIM10->CCR1 = TIM10->CCR1 - .1;
		}
		
	
	//check start/stop the timer
		if( startstop %2 == 0) //if startstop is an even number
			{	
					decitime++;
				//then reset counter after it counts to 9 then 
				if(decitime ==	10)
				{				
					decitime = 0; //reset decimal counter
					alwayscountup(); //add a whole second
					displayLEDs(count); //display it
				}
			
				displayLEDs2(decitime);
			}

		else
		{
			if(keyPressed == 2) //and reset is (or 2) is pressed
			{ 
				//then reset
				decitime = 0; 
				count = 0;
				displayLEDs(count);
				displayLEDs2(decitime);
			}
		}
		
		TIM11-> SR &= ~0x01;
		NVIC_ClearPendingIRQ(TIM11_IRQn);
		TIM11 -> CNT=0; //reset timer
			
		TIM11-> SR &= ~0x01;
		NVIC_ClearPendingIRQ(TIM11_IRQn);
		TIM11 -> CNT=0; //reset timer	
		
}

/* Delay function - do nothing for about 1 second-from lab 1*/
void delay() {
    int i,j,n;
    for (i=0; i<=60; i++) { //outer loop
        for (j=0; j<2700; j++) { //inner loop
            n = j; //dummy operation for single-step test
        } //do nothing
    }
}

int main(void)
{
	/*enable interrupt*/
	__enable_irq();
	
	//set up pins
	pinSetup();
	TIM10->CR1 |= TIM_CR1_CEN; // enable timer for decitime
	TIM11->CR1 |= TIM_CR1_CEN; //enable ADC timer

	
	while(1)
	{

	}
	
	return 0;
}


