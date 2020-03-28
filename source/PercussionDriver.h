/*
 * PercussionDriver.h
 *
 *  Created on: Mar 28, 2020
 *      Author: Stanciu
 */

#ifndef PERCUSSIONDRIVER_H_
#define PERCUSSIONDRIVER_H_
#define PERCUSSION_PINS 2

#include "board.h"
#include "peripherals.h"
#include "MKL25Z4.h"
#include "fsl_gpio.h"

GPIOPin percussionTriggers[] = {{GPIOE, 5}, {GPIOE, 6}};
gpio_pin_config_t percussionPinConfig = {
    		kGPIO_DigitalOutput,
			1,
    };

/*
 * @brief
 * Initialize pins that will control percussion
 *
 */
void InitPercussionDriver(){
	for(int i=0;i<PERCUSSION_PINS;i++){
		GPIO_PinInit(percussionTriggers[i].port, percussionTriggers[i].pin, &percussionPinConfig);
	}

}

/*
 * @brief
 * Flips each pin that is active in bitmask
 */
void ProcessPercussionMessage(uint8_t bitmask){
	/* Flip each masked pin */
	for(int i=0;i<PERCUSSION_PINS;i++){
		if(bitmask %2){ //If bit is set...
			GPIO_TogglePinsOutput(percussionTriggers[i].port, 1<<percussionTriggers[i].pin);
		}
		bitmask = bitmask >> 1; // Shift to the next bit in line
	}
}

#endif /* PERCUSSIONDRIVER_H_ */
