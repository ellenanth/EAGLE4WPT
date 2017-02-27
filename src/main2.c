///**
//  ******************************************************************************
//  * @file    main.c
//  * @author  Ac6
//  * @version V1.0
//  * @date    01-December-2013
//  * @brief   Default main function.
//  ******************************************************************************
//*/
//
//
//#include "stm32f30x.h"
//#include "stm32f3xx_nucleo.h"
//
//
//int main(void)
//{
//
//	initPins();
//	GPIO_SetBits(GPIOA, GPIO_Pin_10); // driver ENABLE
//
//	//init high resolution timer
//	SetTimer(70000, 500);
//
//	//test: wisselen tussen 2 frequenties en duty-cycles door op blauwe knop te drukken
//	int state = !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);
//	for(;;) {
//		if (state != !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)) {
//			state = !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);
//			if (state) {
//				GPIO_SetBits(GPIOA, GPIO_Pin_5);
//			//	GPIO_SetBits(GPIOA, GPIO_Pin_10); // driver ENABLE
//		//		SetTimer(20000, 800);
//			} else {
//				GPIO_ResetBits(GPIOA, GPIO_Pin_5);
//			//	GPIO_ResetBits(GPIOA, GPIO_Pin_10); // driver ENABLE
//		//		SetTimer(20000, 500);
//			}
//		}
//	}
//}
//
//void SetTimer(int frequentie, int duty_cycle) {
//	// duty_cycle: tussen 0 en 1000
//	// !!! laagste frequentie 18 kHz, hoogste +- 200 kHz (door niets aan de prescaler te veranderen) !!!
//	// max periode: +- 65000 ?
//	int twee_f_72M = 2*72000000; // de laagste frequentie
//	int max_periode = 65000;
//	uint32_t prescaler = HRTIM_PRESCALERRATIO_MUL8;
//	int periode = twee_f_72M*8/frequentie;
//	initHighResTimer(periode, prescaler, duty_cycle*periode/2000);
//}
//
////helper function to initialize the high resolution timer
//void initHighResTimer(int periode, uint32_t prescaler_ratio, int puls_periode) {
//	// puls_periode: tijd van de puls voor een 1
//	int BUCK_PWM_PERIOD = periode; // 11520;
//
//	/* Configure HRTIM output */
//	static GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//
//	//init GPIO A pin 8 (TA1)
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	/* Alternate function configuration : HRTIM TA1 (PA8) */
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_13);
//
//	//init GPIO A pin 9 (TA2)
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	/* Alternate function configuration : HRTIM TA2 (PA9) */
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_13);
//
//	//create setup structures
//	HRTIM_OutputCfgTypeDef HRTIM_TIM_OutputStructure;
//	HRTIM_BaseInitTypeDef HRTIM_BaseInitStructure;
//	HRTIM_TimerInitTypeDef HRTIM_TimerInitStructure;
//	HRTIM_TimerCfgTypeDef HRTIM_TimerWaveStructure;
//
//	/* ----------------------------*/
//	/* HRTIM Global initialization */
//	/* ----------------------------*/
//	/* Use the PLLx2 clock for HRTIM */
//	RCC_HRTIM1CLKConfig(RCC_HRTIM1CLK_PLLCLK);
//	/* Enable HRTIM clock*/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_HRTIM1, ENABLE);
//
//	/* HRTIM DLL calibration: periodic calibration, set period to 14?s */
//	HRTIM_DLLCalibrationStart(HRTIM1, HRTIM_CALIBRATIONRATE_14);
//	HRTIM1_COMMON->DLLCR |= HRTIM_DLLCR_CALEN;
//
//	/* Wait calibration completion*/
//	while(HRTIM_GetCommonFlagStatus(HRTIM1, HRTIM_ISR_DLLRDY) == RESET);
//
//
//	/* --------------------------------------------------- */
//	/* TIMERA initialization: timer mode and PWM frequency */
//	/* --------------------------------------------------- */
//	HRTIM_TimerInitStructure.HalfModeEnable = HRTIM_HALFMODE_DISABLED;
//	HRTIM_TimerInitStructure.StartOnSync = HRTIM_SYNCSTART_DISABLED;
//	HRTIM_TimerInitStructure.ResetOnSync = HRTIM_SYNCRESET_DISABLED;
//	HRTIM_TimerInitStructure.DACSynchro = HRTIM_DACSYNC_NONE;
//	HRTIM_TimerInitStructure.PreloadEnable = HRTIM_PRELOAD_ENABLED;
//	HRTIM_TimerInitStructure.UpdateGating = HRTIM_UPDATEGATING_INDEPENDENT;
//	HRTIM_TimerInitStructure.BurstMode = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
//	HRTIM_TimerInitStructure.RepetitionUpdate = HRTIM_UPDATEONREPETITION_ENABLED;
//
//	HRTIM_BaseInitStructure.Period = BUCK_PWM_PERIOD; /* 400kHz switching frequency */
//	HRTIM_BaseInitStructure.RepetitionCounter = 127;   /* 1 ISR every 128 PWM periods */
//	HRTIM_BaseInitStructure.PrescalerRatio = prescaler_ratio; // HRTIM_PRESCALERRATIO_MUL32;
//	HRTIM_BaseInitStructure.Mode = HRTIM_MODE_CONTINOUS;
//
//	HRTIM_Waveform_Init(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, &HRTIM_BaseInitStructure, &HRTIM_TimerInitStructure);
//
//
//
//	/* ------------------------------------------------ */
//	/* TIMERA output and registers update configuration */
//	/* ------------------------------------------------ */
//	HRTIM_TimerWaveStructure.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
//	HRTIM_TimerWaveStructure.DelayedProtectionMode = HRTIM_TIMDELAYEDPROTECTION_DISABLED;
//	HRTIM_TimerWaveStructure.FaultEnable = HRTIM_TIMFAULTENABLE_FAULT1;
//	HRTIM_TimerWaveStructure.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE;
//	HRTIM_TimerWaveStructure.PushPull = HRTIM_TIMPUSHPULLMODE_DISABLED;
//	HRTIM_TimerWaveStructure.ResetTrigger = HRTIM_TIMRESETTRIGGER_NONE;
//	HRTIM_TimerWaveStructure.ResetUpdate = HRTIM_TIMUPDATEONRESET_DISABLED;
//	HRTIM_TimerWaveStructure.UpdateTrigger = HRTIM_TIMUPDATETRIGGER_NONE;
//	HRTIM_WaveformTimerConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, &HRTIM_TimerWaveStructure);
//
//
//	/* -------------------------------- */
//	/* TA1 and TA2 waveform description */
//	/* -------------------------------- */
//	/* PWM on TA1, protected by Fault input */
//	// create general outputStructure
//	HRTIM_TIM_OutputStructure.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
//	HRTIM_TIM_OutputStructure.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
//	HRTIM_TIM_OutputStructure.IdleState = HRTIM_OUTPUTIDLESTATE_INACTIVE;
//	HRTIM_TIM_OutputStructure.FaultState = HRTIM_OUTPUTFAULTSTATE_INACTIVE;
//	HRTIM_TIM_OutputStructure.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
//	HRTIM_TIM_OutputStructure.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
//
//	// change specific setting + use general outputStructure for both TA1
//	HRTIM_TIM_OutputStructure.SetSource = HRTIM_OUTPUTSET_TIMPER;			//rising edge pwm
//	HRTIM_TIM_OutputStructure.ResetSource = HRTIM_OUTPUTRESET_TIMCMP1;		//falling edge pwm
//	HRTIM_WaveformOutputConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TA1, &HRTIM_TIM_OutputStructure);
//
//	// change specific setting + use general outputStructure for both TA2
//	HRTIM_TIM_OutputStructure.SetSource = HRTIM_OUTPUTSET_TIMCMP2;			//rising edge pwm
//	HRTIM_TIM_OutputStructure.ResetSource = HRTIM_OUTPUTRESET_TIMCMP3;		//falling edge pwm
//	HRTIM_WaveformOutputConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TA2, &HRTIM_TIM_OutputStructure);
//
//	/* Set compare registers for duty cycle */
//	HRTIM_CompareCfgTypeDef HRTIM_CompareStructure;
//	HRTIM_CompareStructure.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
//	HRTIM_CompareStructure.AutoDelayedTimeout = 0;
//
//	HRTIM_CompareStructure.CompareValue = puls_periode; // BUCK_PWM_PERIOD/4;
//	HRTIM_WaveformCompareConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1, &HRTIM_CompareStructure);
//
//	HRTIM_CompareStructure.CompareValue = BUCK_PWM_PERIOD/2;
//	HRTIM_WaveformCompareConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_2, &HRTIM_CompareStructure);
//
//	HRTIM_CompareStructure.CompareValue = BUCK_PWM_PERIOD/2 + puls_periode;
//	HRTIM_WaveformCompareConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_3, &HRTIM_CompareStructure);
//
//
//
//	/* ---------------*/
//	/* HRTIM start-up */
//	/* ---------------*/
//	/* Enable HRTIM's outputs TA1 and TA2 */
//	/* Note: it is necessary to enable also GPIOs to have outputs functional */
//	HRTIM_WaveformOutputStart(HRTIM1, HRTIM_OUTPUT_TA1 | HRTIM_OUTPUT_TA2);
//
//	/* Start HRTIM's TIMER A */
//	HRTIM_WaveformCounterStart(HRTIM1, HRTIM_TIMERID_TIMER_A);
//
//}
//
//
//
//void initPins() {
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD, ENABLE); // gewoon nodig, anders werkt het niet
//
//	// driver IN1A en IN1B (PA8) en IN2A en IN2B (PA9)
//	// PA8
//	static GPIO_InitTypeDef GPIO_IN1;
//	GPIO_IN1.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_IN1.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_IN1.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_IN1.GPIO_OType = GPIO_OType_PP;
//	GPIO_IN1.GPIO_Pin = GPIO_Pin_8;
//	GPIO_Init(GPIOA, &GPIO_IN1);
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_13); // Alternate function configuration : HRTIM TA1 (PA8)
//	// PA9
//	GPIO_IN1.GPIO_Pin = GPIO_Pin_9;
//	GPIO_Init(GPIOA, &GPIO_IN1);
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_13); // Alternate function configuration : HRTIM TA2 (PA9)
//
//	// driver ENA, ENB, OCDA en OCDB
//	// PA10
//	static GPIO_InitTypeDef GPIO_EN;
//	GPIO_EN.GPIO_Speed = GPIO_Speed_Level_3;
//	GPIO_EN.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_EN.GPIO_OType = GPIO_OType_PP;
//	GPIO_EN.GPIO_Pin = GPIO_Pin_10;
//	GPIO_Init(GPIOA, &GPIO_EN);
//
//	// transformator middelste pin spanning weerstanden
//	// PA5
//	static GPIO_InitTypeDef GPIO_TR_R;
//	GPIO_TR_R.GPIO_Speed = GPIO_Speed_Level_3;
//	GPIO_TR_R.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_TR_R.GPIO_OType = GPIO_OType_PP;
//	GPIO_TR_R.GPIO_Pin = GPIO_Pin_5;
//	GPIO_Init(GPIOA, &GPIO_TR_R);
//
//	// transformator analoge pinnen over de weerstand
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);
//	// PA0 en PA1
//	static GPIO_InitTypeDef GPIO_TR_An;
//	GPIO_TR_An.GPIO_Speed = GPIO_Speed_Level_3;
//	GPIO_TR_An.GPIO_Mode = GPIO_Mode_AN;
//	GPIO_TR_An.GPIO_OType = GPIO_PuPd_NOPULL;
//	GPIO_TR_An.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
//	GPIO_Init(GPIOA, &GPIO_TR_An);
//	ADC_InitTypeDef ADC_InitStructure;
//	ADC_StructInit(&ADC_InitStructure);
//	/* Calibration procedure */
//	ADC_VoltageRegulatorCmd(ADC1, ENABLE);
//	Delay(10); // Insert delay equal to 10 µs
//	ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Differential);
//	ADC_StartCalibration(ADC1);
//	while(ADC_GetCalibrationStatus(ADC1) != RESET );
//	uint32_t calibration_value = 0;
//	calibration_value = ADC_GetCalibrationValue(ADC1);
//	ADC_CommonInitTypeDef ADC_CommonInitStructure;
//	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
//	ADC_CommonInitStructure.ADC_Clock = ADC_Clock_AsynClkMode;
//	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
//	ADC_CommonInitStructure.ADC_DMAMode = ADC_DMAMode_OneShot;
//	ADC_CommonInitStructure.ADC_TwoSamplingDelay = 0;
//	ADC_CommonInit(ADC1, &ADC_CommonInitStructure);
//	ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Enable;
//	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
//	ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_0;
//	ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_None;
//	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//	ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Disable;
//	ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;
//	ADC_InitStructure.ADC_NbrOfRegChannel = 1;
//	ADC_Init(ADC1, &ADC_InitStructure);
//	/* ADC1 regular channel7 and channel8 configuration */
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_7Cycles5);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 2, ADC_SampleTime_7Cycles5);
//	/* Select the differetiel mode for Channel 7 */
//	ADC_SelectDifferentialMode(ADC1, ADC_Channel_7, ENABLE);
//	/* Enable ADC1 */
//	ADC_Cmd(ADC1, ENABLE);
//	/* wait for ADRDY */
//	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));
//	/* Start ADC1 Software Conversion */
//	ADC_StartConversion(ADC1);
//
//
//	// de blauwe knop op de Nucleo
//	// PC13
//	static GPIO_InitTypeDef GPIO_knop;
//	GPIO_knop.GPIO_Speed = GPIO_Speed_Level_3;
//	GPIO_knop.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_knop.GPIO_OType = GPIO_OType_PP;
//	GPIO_knop.GPIO_Pin = GPIO_Pin_13;
//	GPIO_Init(GPIOC, &GPIO_knop);
//}
//
//int SpanningRTransfo() {
//	return ADC_GetConversionValue(ADC1);//*6600/0xFFF;
//}
//
//
