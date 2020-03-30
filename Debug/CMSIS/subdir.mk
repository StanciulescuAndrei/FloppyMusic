################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/system_MKL25Z4.c 

OBJS += \
./CMSIS/system_MKL25Z4.o 

C_DEPS += \
./CMSIS/system_MKL25Z4.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/%.o: ../CMSIS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/home/stanciu/Documents/MCUXpresso/workspace/FloppyMusic/drivers" -I"/home/stanciu/Documents/MCUXpresso/workspace/FloppyMusic/CMSIS" -I"/home/stanciu/Documents/MCUXpresso/workspace/FloppyMusic/utilities" -I"/home/stanciu/Documents/MCUXpresso/workspace/FloppyMusic/drivers" -I"/home/stanciu/Documents/MCUXpresso/workspace/FloppyMusic/CMSIS" -I"/home/stanciu/Documents/MCUXpresso/workspace/FloppyMusic/utilities" -I"/home/stanciu/Documents/MCUXpresso/workspace/FloppyMusic/board" -I"/home/stanciu/Documents/MCUXpresso/workspace/FloppyMusic/source" -I"/home/stanciu/Documents/MCUXpresso/workspace/FloppyMusic" -I"/home/stanciu/Documents/MCUXpresso/workspace/FloppyMusic/startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


