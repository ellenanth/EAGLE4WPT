/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f30x.h"
#include "stm32f3xx_nucleo.h"
			

int main(void)
{
	setupGPIO();
	InitializeTimer();

	for (;;)
	{
		int timerValue = TIM_GetCounter(TIM2);
		if (timerValue == 400)
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
		else if (timerValue == 500)
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
	}
}

void setupGPIO()
{
	//GPIO AHB: enable clock van gpio voor led
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
		//pin 5 op input instellen (output is standaard) voor LED
		GPIO_InitTypeDef gpioinit;
		//opties: input, output, alternate function (AF), analog (AN)
		gpioinit.GPIO_Mode = GPIO_Mode_OUT;
		//opties: push pull, open drain -> altijd push pull
		gpioinit.GPIO_OType = GPIO_OType_PP;
		gpioinit.GPIO_Pin = GPIO_Pin_5;
		//pull up - pull down
		gpioinit.GPIO_PuPd = GPIO_PuPd_NOPULL;
		gpioinit.GPIO_Speed = GPIO_Speed_Level_3;

		//initialisatie afronden
		GPIO_Init(GPIOA, &gpioinit);
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
}

void InitializeTimer()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef timerInitStructure;
    timerInitStructure.TIM_Prescaler = 40000;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = 500;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &timerInitStructure);
    TIM_Cmd(TIM2, ENABLE);
}
