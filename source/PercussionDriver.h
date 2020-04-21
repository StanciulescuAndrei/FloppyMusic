/*
 * PercussionDriver.h
 *
 *  Created on: Mar 28, 2020
 *      Author: Stanciu
 *
 *  @brief
 *  The library should manage all interfaces to the percussion library. For now it only flips bits whenever
 *  a new nessage arrives, so the instrument partitioning should be done by the master.
 */

#ifndef PERCUSSIONDRIVER_H_
#define PERCUSSIONDRIVER_H_
#define PERCUSSION_PINS 4

#include "board.h"
#include "peripherals.h"
#include "MKL25Z4.h"
#include "fsl_gpio.h"
#include "fsl_lptmr.h"
#include "pin_mux.h"
#include "clock_config.h"

#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_PllFllSelClk)
#define LPTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_LpoClk)

GPIOPin percussionDirection[] = {{GPIOE, 5}, {GPIOE, 4}, {GPIOE, 3}, {GPIOE, 2}};

/* Monostable simulator: one shot timed pulse */
void LPTMR0_IRQHandler(void)
{
    LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);

    for(int i=0;i<PERCUSSION_PINS;i++){
    	GPIO_WritePinOutput(percussionDirection[i].port, percussionDirection[i].pin, 0);
    }

    LPTMR_StopTimer(LPTMR0);

}

/*
 * @brief
 * Initialize pins that will control percussion
 *
 */
void InitPercussionDriver(){
	tpm_config_t tpmInfo;
	tpm_chnl_pwm_signal_param_t tpmParam;
	tpm_pwm_level_select_t pwmLevel = kTPM_HighTrue;

	TPM_GetDefaultConfig(&tpmInfo);

	/* First PWM channel */
	tpmParam.chnlNumber = (tpm_chnl_t)4U;
	tpmParam.level = pwmLevel;
	tpmParam.dutyCyclePercent = 75U; /*Max duty cycle*/

	TPM_Init(TPM0, &tpmInfo);
	TPM_SetupPwm(TPM0, &tpmParam, 1U, kTPM_EdgeAlignedPwm, 24000U, TPM_SOURCE_CLOCK);
	TPM_StartTimer(TPM0, kTPM_SystemClock);

	/* First PWM channel */
	tpmParam.chnlNumber = (tpm_chnl_t)0U;
	tpmParam.level = pwmLevel;
	tpmParam.dutyCyclePercent = 75U; /*Max duty cycle*/

	TPM_Init(TPM1, &tpmInfo);
	TPM_SetupPwm(TPM1, &tpmParam, 1U, kTPM_EdgeAlignedPwm, 24000U, TPM_SOURCE_CLOCK);
	TPM_StartTimer(TPM1, kTPM_SystemClock);

	/* Setup LPTMR */
	lptmr_config_t lptmrConfig;
	LPTMR_GetDefaultConfig(&lptmrConfig);
	LPTMR_Init(LPTMR0, &lptmrConfig);
	LPTMR_SetTimerPeriod(LPTMR0, USEC_TO_COUNT(10000, LPTMR_SOURCE_CLOCK));
	LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
	EnableIRQ(LPTMR0_IRQn);

}

/*
 * @brief
 * Flips each pin that is active in bitmask
 */
void ProcessPercussionMessage(uint8_t bitmask){
	/* Flip each masked pin */
	for(int i=0;i<PERCUSSION_PINS/2;i++){
		if(bitmask %2){ //If bit is set...
			GPIO_SetPinsOutput(percussionDirection[i*2].port, 1<<percussionDirection[i*2].pin);
		}
		bitmask = bitmask >> 1; // Shift to the next bit in line
	}
	/*Pornim un timer si la interrupt oprim alimentarea la HDD*/

	LPTMR_StartTimer(LPTMR0);
}

#endif /* PERCUSSIONDRIVER_H_ */
