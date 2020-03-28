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
