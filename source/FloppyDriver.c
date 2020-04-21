/*
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    FloppyDriver.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "fsl_pit.h"
#include "fsl_uart.h"
#include "fsl_tpm.h"

#include "FloppyDeviceDriver.h"
#include "I2CObserver.h"
#include "PercussionDriver.h"


#define PERCUSSION_ID 0xf0
#define PulseGenerator PIT_IRQHandler

float midi[127];

/* Gobal device driver variables */
FloppyDeviceDriver fdd[2];

void PulseGenerator(){
	uint8_t channel;
	for(int i=0;i<2;i++){
		if(PIT_GetStatusFlags(PIT, i) != 0){
			channel = i;
			/* Clear interrupt flag */
			PIT_ClearStatusFlags(PIT, channel, kPIT_TimerFlag);
			/* Step the appropriate floppy */
			for(int i=0;i<NOF;i++){
				if(fdd[i].channel == channel){
					FDDStep(&(fdd[i]));
				}
			}

		}
	}


	GPIO_TogglePinsOutput(GPIOB, 1<<19U);

	/*
	__DSB();
	__ISB();
	*/
}

/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    PRINTF("Floppy driver started...\r\n");

    /*
     * Status LED Configuration
     * PTB19, active low
     */
    gpio_pin_config_t statusLedConfig = {
    		kGPIO_DigitalOutput,
			1,
    };
    GPIO_PinInit(GPIOB, 19, &statusLedConfig);

    /*
     * Set up the Floppy Drive interface
     */
    InitFloppyInterface(fdd, midi);
    /*
    * PIT Configuration
    * Timer interrupt is used to generate the
    * base frequency for the FDD step pulses.
    * The timer interval is defined as 1 / baseFrequency
    *
    * Each of the 2 PIT channels is assigned to a Floppy device so each has its own interrupt.
    * This way we can generate the frequencies without having a master frequency and a divider
    */

    pit_config_t pitConfig;
    PIT_GetDefaultConfig(&pitConfig);
    PIT_Init(PIT, &pitConfig);
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
    PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);
    EnableIRQ(PIT_IRQn);

    PRINTF("Periodic Timer Interrupt Initialized: %d MHz\r\n", CLOCK_GetFreq(kCLOCK_BusClk)/1000000U);
    FDDSetFrequency(0, 1);
    FDDSetFrequency(1, 1);


    /*
     * Initialize percussion drivers
     *
     */
    InitPercussionDriver();

    /*
     * Initialize I2C device driver with buffers and all the good stuff
     */
    InitI2CInterface();
    PRINTF("I2C bus initialized\r\n");

    /*
     * Main control loop for parsing the data coming in through serial
     * and setting the appropriate frequencies or stopping the drives
     */
    /*
     *
     * Structura fiecarui frame: CC NN; CC este canalul indexat de la 1, de aceea scadem 1, N e nota in index midi
     *
     */
    while(1) {
    	size_t recvFrames = I2CReceiveData() / 2;
    	if(recvFrames > I2C_DATA_LENGTH - 1)
    		continue;/*Invalid frame*/
    	uint8_t * dataBuffer = slaveBuffer + 1;
    	for(int i = 0;i<recvFrames;i++){
    		PRINTF("%02X %02X\r\n", dataBuffer[i*2] - 1, dataBuffer[1 + i*2]);
    		if(dataBuffer[1 + i*2] == 255)
    			dataBuffer[1 + i*2] = 0; /* 0xff e codul pentru oprirea notei */
    		if(dataBuffer[i*2] == PERCUSSION_ID){
    			ProcessPercussionMessage(dataBuffer[i*2 + 1]);
    		}
    		else{
    			FDDSetFrequency(dataBuffer[i*2] - 1, (float)(midi[dataBuffer[1 + i*2]]));
    		}

    	}
    }
    return (0);
}
