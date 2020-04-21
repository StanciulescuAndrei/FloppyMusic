#include "FloppyDeviceDriver.h"

/*
 * @brief Some delay function so that pulses are not too fast
 *
 * @param ticks Arbitrary number of ticks, has nu real correlation to time measurement units
 */

void delay(int ticks) {
	volatile int i = 0;
	for (i = 0; i < ticks * 1000; i++) {
		__asm("NOP");
	}
}

void FDDStep(FloppyDeviceDriver *device) {
	/* Increment absolute position */
	device->crtPos++;
	/* Write the STEP pin high */
	GPIO_WritePinOutput(device->stepPin.port, device->stepPin.pin, 1);
	delay(3);
	/* Write the STEP pin low */
	GPIO_WritePinOutput(device->stepPin.port, device->stepPin.pin, 0);

	/* Reverse direction when it reaches end of tracks */
	if (device->crtPos == 80) {
		device->crtPos = 1;
		device->direction = !device->direction;
		GPIO_WritePinOutput(device->dirPin.port, device->dirPin.pin, device->direction);
	}
}

void FDDInit(FloppyDeviceDriver *device, GPIOPin stepPin, GPIOPin dirPin, uint8_t channel) {
	device->direction = 0;
	device->crtPos = 0;
	device->channel = channel;

	device->stepPin = stepPin;
	device->dirPin = dirPin;
}

void FDDReset(FloppyDeviceDriver *device) {
	/* Set direction to 1 */
	GPIO_WritePinOutput(device->dirPin.port, device->dirPin.pin, 1);
	/* Force 80 steps in the same direction */
	for (int i = 0; i < 80; i++) {
		GPIO_WritePinOutput(device->stepPin.port, device->stepPin.pin, 1);
		delay(30);
		GPIO_WritePinOutput(device->stepPin.port, device->stepPin.pin, 0);
		delay(30);
	}

	/* Reinit position stats */
	GPIO_WritePinOutput(device->dirPin.port, device->dirPin.pin, 0);
	device->crtPos = 1;
	device->direction = 0;
}

void FDDSetFrequency(int channelID, float frequency){
	if(frequency < 0){
		PIT_StopTimer(PIT, channelID);
	}
	else{
		 PIT_SetTimerPeriod(PIT, channelID, USEC_TO_COUNT(1000000U / frequency * 2, CLOCK_GetFreq(kCLOCK_BusClk)));
		 PIT_StartTimer(PIT, channelID);
	}
}

void InitFloppyInterface(FloppyDeviceDriver* fdd, float * midi){

    /*
     * FloppyDeviceDriver initialization
     * Static pin assignments for now
     */

    GPIOPin stepLanes[] = {{PTC, 0}, {PTC, 4}, {PTC, 6}, {PTC, 11}, {PTC, 13}, {PTC, 17}};
    GPIOPin directionLanes[] = {{PTC, 7}, {PTC, 3}, {PTC, 5}, {PTC, 10}, {PTC, 12}, {PTC, 16}};

    /*
     * Assign first half to the first channel, second half to second channel
     * Initialize and reset floppys to start position
     */

    for(int channel=0;channel<INTERRUPT_CHANNELS;channel++){
    	for(int index = 0;index<NOF/INTERRUPT_CHANNELS;index++){
    		int absoluteIndex = channel * NOF/INTERRUPT_CHANNELS;
    		FDDInit(&(fdd[absoluteIndex]), stepLanes[absoluteIndex], directionLanes[absoluteIndex], channel);
    		FDDReset(&(fdd[absoluteIndex]));
    	}
    }

    /*
     * Prepare MIDI lookup table
     */

    int a = 440; // a is 440 hz...
    for (int x = 0; x < 127; ++x)
    {
       midi[x] = (a / 32) * (pow(((double)2), ((double)(x - 9)) / 12));
    }
    midi[0] = -1.00f;
}

