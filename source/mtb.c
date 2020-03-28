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
 * @file    mtb.c
 * @brief   MTB initialization file.
 * @details Symbols controlling behavior of this code...
 * 			__MTB_DISABLE
 *     		If this symbol is defined, then the buffer array for the MTB
 *     		will not be created.
 *
 * 			__MTB_BUFFER_SIZE
 *     		Symbol specifying the sizer of the buffer array for the MTB.
 *     		This must be a power of 2 in size, and fit into the available
 *   		RAM. The MTB buffer will also be aligned to its 'size' 
 *     		boundary and be placed at the start of a RAM bank (which 
 *     		should ensure minimal or zero padding due to alignment).
 * 
 * 			__MTB_RAM_BANK
 *     		Allows MTB Buffer to be placed into specific RAM bank. When 
 *     		this is not defined, the "default" (first if there are 
 *     		several) RAM bank is used.
 */
 
/* This is a template for board specific configuration created by MCUXpresso IDE Project Wizard.*/


#if !defined (__MTB_DISABLE)

  #if !defined (__MTB_BUFFER_SIZE)
    #define __MTB_BUFFER_SIZE 128
  #endif
  

  #if (__MTB_BUFFER_SIZE > 0)

    #include <cr_mtb_buffer.h>


    #if defined(__MTB_RAM_BANK)
	    __CR_MTB_BUFFER_EXT(__MTB_BUFFER_SIZE,__MTB_RAM_BANK);
    #else
	    __CR_MTB_BUFFER(__MTB_BUFFER_SIZE);

    #endif

  #endif

#endif

