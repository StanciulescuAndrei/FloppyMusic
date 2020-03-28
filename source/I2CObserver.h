/*
 * I2CObserver.h
 *
 *  Created on: Mar 21, 2020
 *      Author: Stanciu
 */
#pragma once
#ifndef I2COBSERVER_H_
#define I2COBSERVER_H_
#include "fsl_i2c.h"

#define I2C_SLAVE_CLK_FREQ CLOCK_GetFreq(I2C1_CLK_SRC)
#define I2C_SLAVE_IRQ I2C1_IRQn
#define I2C_DATA_LENGTH 32U
#define I2C_SLAVE_ADDR_7BIT 0x69
#define I2C_SLAVE_BASEADDR I2C1

static uint8_t slaveBuffer[I2C_DATA_LENGTH];
static i2c_slave_handle_t slaveHandle;

volatile bool slaveCompletionFlag = false;

static void i2c_slave_callback(I2C_Type *base, i2c_slave_transfer_t *xfer, void *userData)
{
    switch (xfer->event)
    {
        /*  Transmit request */
        case kI2C_SlaveTransmitEvent:
            /*  Update information for transmit process */
            xfer->data = slaveBuffer;
            xfer->dataSize = I2C_DATA_LENGTH;
            break;

        /*  Receive request */
        case kI2C_SlaveReceiveEvent:
            /*  Update information for received process */
            xfer->data = slaveBuffer;
            xfer->dataSize = I2C_DATA_LENGTH;
            break;

        /*  Transfer done */
        case kI2C_SlaveCompletionEvent:
        	slaveCompletionFlag = true;
            break;

        default:
        	slaveCompletionFlag = false;
            break;
    }
}

void InitI2CInterface(){
		i2c_slave_config_t slaveConfig;

	    /*  Set i2c slave interrupt priority higher. */
	    NVIC_SetPriority(I2C_SLAVE_IRQ, 0);

	    /*1.Set up i2c slave first*/
	    I2C_SlaveGetDefaultConfig(&slaveConfig);

	    slaveConfig.addressingMode = kI2C_Address7bit;
	    slaveConfig.slaveAddress = I2C_SLAVE_ADDR_7BIT;
	    slaveConfig.upperAddress = 0; /*  not used for this example */

	    I2C_SlaveInit(I2C_SLAVE_BASEADDR, &slaveConfig, I2C_SLAVE_CLK_FREQ);

	    for (uint32_t i = 0U; i < I2C_DATA_LENGTH; i++)
	    {
	    	slaveBuffer[i] = 0;
	    }

	    memset(&slaveHandle, 0, sizeof(slaveHandle));

	    I2C_SlaveTransferCreateHandle(I2C_SLAVE_BASEADDR, &slaveHandle, i2c_slave_callback, NULL);
}

size_t I2CReceiveData(){
	slaveCompletionFlag = false;
	memset(slaveBuffer, 0, I2C_DATA_LENGTH);
	I2C_SlaveTransferNonBlocking(I2C_SLAVE_BASEADDR, &slaveHandle, kI2C_SlaveCompletionEvent);
	while(!slaveCompletionFlag); /* Wait for transfer to end */
	return (strlen(slaveBuffer + 1));

}

#endif /* I2COBSERVER_H_ */
