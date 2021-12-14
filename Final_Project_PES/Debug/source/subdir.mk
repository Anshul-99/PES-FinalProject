################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/cbfifo.c \
../source/command_processor.c \
../source/display.c \
../source/hexdump.c \
../source/i2c.c \
../source/main.c \
../source/mtb.c \
../source/oled.c \
../source/semihost_hardfault.c \
../source/sysclock.c \
../source/uart.c 

OBJS += \
./source/cbfifo.o \
./source/command_processor.o \
./source/display.o \
./source/hexdump.o \
./source/i2c.o \
./source/main.o \
./source/mtb.o \
./source/oled.o \
./source/semihost_hardfault.o \
./source/sysclock.o \
./source/uart.o 

C_DEPS += \
./source/cbfifo.d \
./source/command_processor.d \
./source/display.d \
./source/hexdump.d \
./source/i2c.d \
./source/main.d \
./source/mtb.d \
./source/oled.d \
./source/semihost_hardfault.d \
./source/sysclock.d \
./source/uart.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"/home/anshul/CU/Courses/PES/Workspace_PES/Final_Project_PES/board" -I"/home/anshul/CU/Courses/PES/Workspace_PES/Final_Project_PES/source" -I"/home/anshul/CU/Courses/PES/Workspace_PES/Final_Project_PES" -I"/home/anshul/CU/Courses/PES/Workspace_PES/Final_Project_PES/drivers" -I"/home/anshul/CU/Courses/PES/Workspace_PES/Final_Project_PES/CMSIS" -I"/home/anshul/CU/Courses/PES/Workspace_PES/Final_Project_PES/utilities" -I"/home/anshul/CU/Courses/PES/Workspace_PES/Final_Project_PES/startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


