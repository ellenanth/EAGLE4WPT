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
	//enable clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	//setup GPIO A pin 5
	GPIO_InitTypeDef gpioInit;
	gpioInit.GPIO_Pin = GPIO_Pin_5;
	gpioInit.GPIO_Mode = GPIO_Mode_OUT;
	gpioInit.GPIO_Speed = GPIO_Speed_Level_3;
	gpioInit.GPIO_OType = GPIO_OType_PP;

	GPIO_Init(GPIOA,&gpioInit);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);

	//init high resolution timer
	hrtimInit();

	for(;;){
	}
}

//helper function to initialize the high resolution timer
void hrtimInit(void){
	int BUCK_PWM_PERIOD = 11520;

	static GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);

	/* Configure HRTIM output: TA1 (PA8) */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Alternate function configuration : HRTIM TA1 (PA8) */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_13);

	/* Configure HRTIM output: TA2 (PA9) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Alternate function configuration : HRTIM TA2 (PA9) */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_13);

	// Init hrtim


	HRTIM_OutputCfgTypeDef HRTIM_TIM_OutputStructure;
	HRTIM_BaseInitTypeDef HRTIM_BaseInitStructure;
	HRTIM_TimerInitTypeDef HRTIM_TimerInitStructure;
	HRTIM_TimerCfgTypeDef HRTIM_TimerWaveStructure;
	HRTIM_CompareCfgTypeDef HRTIM_CompareStructure;

	/* ----------------------------*/
	/* HRTIM Global initialization */
	/* ----------------------------*/
	/* Use the PLLx2 clock for HRTIM */
	RCC_HRTIM1CLKConfig(RCC_HRTIM1CLK_PLLCLK);
	/* Enable HRTIM clock*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_HRTIM1, ENABLE);

	/* HRTIM DLL calibration: periodic calibration, set period to 14?s */
	HRTIM_DLLCalibrationStart(HRTIM1, HRTIM_CALIBRATIONRATE_14);
	HRTIM1_COMMON->DLLCR |= HRTIM_DLLCR_CALEN;

	/* Wait calibration completion*/
	while(HRTIM_GetCommonFlagStatus(HRTIM1, HRTIM_ISR_DLLRDY) == RESET);


	/* --------------------------------------------------- */
	/* TIMERA initialization: timer mode and PWM frequency */
	/* --------------------------------------------------- */
	HRTIM_TimerInitStructure.HalfModeEnable = HRTIM_HALFMODE_DISABLED;
	HRTIM_TimerInitStructure.StartOnSync = HRTIM_SYNCSTART_DISABLED;
	HRTIM_TimerInitStructure.ResetOnSync = HRTIM_SYNCRESET_DISABLED;
	HRTIM_TimerInitStructure.DACSynchro = HRTIM_DACSYNC_NONE;
	HRTIM_TimerInitStructure.PreloadEnable = HRTIM_PRELOAD_ENABLED;
	HRTIM_TimerInitStructure.UpdateGating = HRTIM_UPDATEGATING_INDEPENDENT;
	HRTIM_TimerInitStructure.BurstMode = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
	HRTIM_TimerInitStructure.RepetitionUpdate = HRTIM_UPDATEONREPETITION_ENABLED;

	HRTIM_BaseInitStructure.Period = BUCK_PWM_PERIOD; /* 400kHz switching frequency */
	HRTIM_BaseInitStructure.RepetitionCounter = 127;   /* 1 ISR every 128 PWM periods */
	HRTIM_BaseInitStructure.PrescalerRatio = HRTIM_PRESCALERRATIO_MUL32;
	HRTIM_BaseInitStructure.Mode = HRTIM_MODE_CONTINOUS;

	HRTIM_Waveform_Init(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, &HRTIM_BaseInitStructure, &HRTIM_TimerInitStructure);



	/* ------------------------------------------------ */
	/* TIMERA output and registers update configuration */
	/* ------------------------------------------------ */
	HRTIM_TimerWaveStructure.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
	HRTIM_TimerWaveStructure.DelayedProtectionMode = HRTIM_TIMDELAYEDPROTECTION_DISABLED;
	HRTIM_TimerWaveStructure.FaultEnable = HRTIM_TIMFAULTENABLE_FAULT1;
	HRTIM_TimerWaveStructure.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE;
	HRTIM_TimerWaveStructure.PushPull = HRTIM_TIMPUSHPULLMODE_DISABLED;
	HRTIM_TimerWaveStructure.ResetTrigger = HRTIM_TIMRESETTRIGGER_NONE;
	HRTIM_TimerWaveStructure.ResetUpdate = HRTIM_TIMUPDATEONRESET_DISABLED;
	HRTIM_TimerWaveStructure.UpdateTrigger = HRTIM_TIMUPDATETRIGGER_NONE;
	HRTIM_WaveformTimerConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, &HRTIM_TimerWaveStructure);


	/* -------------------------------- */
	/* TA1 and TA2 waveform description */
	/* -------------------------------- */
	/* PWM on TA1, protected by Fault input */
	HRTIM_TIM_OutputStructure.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
	HRTIM_TIM_OutputStructure.SetSource = HRTIM_OUTPUTSET_TIMPER;
	HRTIM_TIM_OutputStructure.ResetSource = HRTIM_OUTPUTRESET_TIMCMP1;
	HRTIM_TIM_OutputStructure.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
	HRTIM_TIM_OutputStructure.IdleState = HRTIM_OUTPUTIDLESTATE_INACTIVE;
	HRTIM_TIM_OutputStructure.FaultState = HRTIM_OUTPUTFAULTSTATE_INACTIVE;
	HRTIM_TIM_OutputStructure.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
	HRTIM_TIM_OutputStructure.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
	HRTIM_WaveformOutputConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TA1, &HRTIM_TIM_OutputStructure);

	/* -------------------------------- */
	/* TA1 and TA2 waveform description */
	/* -------------------------------- */
	/* PWM on TA1, protected by Fault input */
	HRTIM_TIM_OutputStructure.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
	HRTIM_TIM_OutputStructure.SetSource = HRTIM_OUTPUTSET_TIMPER;
	HRTIM_TIM_OutputStructure.ResetSource = HRTIM_OUTPUTRESET_TIMCMP1;
	HRTIM_TIM_OutputStructure.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
	HRTIM_TIM_OutputStructure.IdleState = HRTIM_OUTPUTIDLESTATE_INACTIVE;
	HRTIM_TIM_OutputStructure.FaultState = HRTIM_OUTPUTFAULTSTATE_INACTIVE;
	HRTIM_TIM_OutputStructure.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
	HRTIM_TIM_OutputStructure.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
	HRTIM_WaveformOutputConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TD1, &HRTIM_TIM_OutputStructure);

	/* Set compare registers for duty cycle on TA1 */
	HRTIM_CompareStructure.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
	HRTIM_CompareStructure.AutoDelayedTimeout = 0;
	HRTIM_CompareStructure.CompareValue = BUCK_PWM_PERIOD/10;     /* 5% duty cycle */
	HRTIM_WaveformCompareConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1, &HRTIM_CompareStructure);



	/* --------------------------*/
	/* ADC trigger initialization */
	/* --------------------------*/
	/* Set compare 3 registers for ADC trigger */
	HRTIM_CompareStructure.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
	HRTIM_CompareStructure.AutoDelayedTimeout = 0;
	HRTIM_CompareStructure.CompareValue = BUCK_PWM_PERIOD/10; /* samples in middle of duty cycle */
	HRTIM_WaveformCompareConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_2, &HRTIM_CompareStructure);


	/* ---------------*/
	/* HRTIM start-up */
	/* ---------------*/
	/* Enable HRTIM's outputs TA1 and TA2 */
	/* Note: it is necessary to enable also GPIOs to have outputs functional */
	HRTIM_WaveformOutputStart(HRTIM1, HRTIM_OUTPUT_TA1 | HRTIM_OUTPUT_TA2);

	/* Start HRTIM's TIMER A */
	HRTIM_WaveformCounterStart(HRTIM1, HRTIM_TIMERID_TIMER_A);

}
