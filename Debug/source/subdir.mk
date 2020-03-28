################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/FloppyDeviceDriver.c \
../source/FloppyDriver.c \
../source/mtb.c \
../source/semihost_hardfault.c 

OBJS += \
./source/FloppyDeviceDriver.o \
./source/FloppyDriver.o \
./source/mtb.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/FloppyDeviceDriver.d \
./source/FloppyDriver.d \
./source/mtb.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\Stanciu\Documents\MCUXpressoIDE_10.3.1_2233\workspace\FloppyDriver\drivers" -I"C:\Users\Stanciu\Documents\MCUXpressoIDE_10.3.1_2233\workspace\FloppyDriver\CMSIS" -I"C:\Users\Stanciu\Documents\MCUXpressoIDE_10.3.1_2233\workspace\FloppyDriver\utilities" -I"C:\Users\Stanciu\Documents\MCUXpressoIDE_10.3.1_2233\workspace\FloppyDriver\drivers" -I"C:\Users\Stanciu\Documents\MCUXpressoIDE_10.3.1_2233\workspace\FloppyDriver\CMSIS" -I"C:\Users\Stanciu\Documents\MCUXpressoIDE_10.3.1_2233\workspace\FloppyDriver\utilities" -I"C:\Users\Stanciu\Documents\MCUXpressoIDE_10.3.1_2233\workspace\FloppyDriver\board" -I"C:\Users\Stanciu\Documents\MCUXpressoIDE_10.3.1_2233\workspace\FloppyDriver\source" -I"C:\Users\Stanciu\Documents\MCUXpressoIDE_10.3.1_2233\workspace\FloppyDriver" -I"C:\Users\Stanciu\Documents\MCUXpressoIDE_10.3.1_2233\workspace\FloppyDriver\startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


