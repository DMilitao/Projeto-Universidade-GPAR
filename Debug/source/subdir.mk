################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/ADC_FFT.c \
../source/mtb.c \
../source/semihost_hardfault.c 

OBJS += \
./source/ADC_FFT.o \
./source/mtb.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/ADC_FFT.d \
./source/mtb.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DDEBUG -DFRDM_KL25Z -DFREEDOM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -D__REDLIB__ -I"C:\Users\User\Documents\MCUXpressoIDE_11.4.0_6237\workspace\FFT\board" -I"C:\Users\User\Documents\MCUXpressoIDE_11.4.0_6237\workspace\FFT\source" -I"C:\Users\User\Documents\MCUXpressoIDE_11.4.0_6237\workspace\FFT" -I"C:\Users\User\Documents\MCUXpressoIDE_11.4.0_6237\workspace\FFT\CMSIS" -I"C:\Users\User\Documents\MCUXpressoIDE_11.4.0_6237\workspace\FFT\drivers" -I"C:\Users\User\Documents\MCUXpressoIDE_11.4.0_6237\workspace\FFT\utilities" -I"C:\Users\User\Documents\MCUXpressoIDE_11.4.0_6237\workspace\FFT\startup" -O0 -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


