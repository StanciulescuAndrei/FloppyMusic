/* @file
 * @brief Base FDD driver file
 *
 * All the data structures, variables and functions that work
 * with the floppy drives will be implemented here for now
 */
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "fsl_pit.h"

#include <math.h>

#define NOF 2 /* Merge pana la 6 cand o sa am atatea */
#define INTERRUPT_CHANNELS 2



/*
 *@brief GPIO structure that holds the port and the pin number for easier access
 */
typedef struct GPIOPin{
	GPIO_Type * port; /*!< GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)  */
	uint32_t pin;     /*!< GPIO pin number */
}GPIOPin;

/*
 *@brief Structure that holds pin information and floppy drive states
 */
typedef struct FloppyDeviceDriver{
	uint16_t crtPos;    /*!< The current position of the read head */
	uint8_t direction;  /*!< Specifies the head direction, which should change every 80 steps in the same direction */
	uint8_t channel;    /*!< PIT interrupt channel */
	GPIOPin stepPin;    /*!< Pin structure for sending the step pulses */
	GPIOPin dirPin;     /*!< Pin structure for the direction signal */
}FloppyDeviceDriver;

/*
 * @brief Initialize the floppies, assign the pins to the channels and set up the timer interrupts
 *
 */

void InitFloppyInterface(FloppyDeviceDriver* fdd, float * midi);

/*
 * @brief Function that performs a step of the floppy read head
 *
 * @param device Pointer to the device that will perform the step
 */
void FDDStep(FloppyDeviceDriver * device);

/*
 * @brief Function that initializes the FloppyDeviceDriver
 *
 * @param device Pointer to the device that will be initialized
 */
void FDDInit(FloppyDeviceDriver * device, GPIOPin stepPin, GPIOPin dirPin, uint8_t channel);

/*
 * @brief Force the read head to the start position to get accurate step positions
 *
 * @param device Pointer to the device that will be reset
 */

void FDDReset(FloppyDeviceDriver * device);

/*
 * @brief Set desired frequency for the PIT channel
 * If frequency is -1, then the drive is disabled
 */

void FDDSetFrequency(int channelID, float frequency);

/*
 *@brief Timer interrupt function
 *
 * The function is called for every interrupt the LPTMR generates and updates every floppy drive
 */
void PulseGenerator();






