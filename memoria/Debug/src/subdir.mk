################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/initial_setup.c \
../src/memoria.c \
../src/server_utils.c \
../src/utils.c 

C_DEPS += \
./src/initial_setup.d \
./src/memoria.d \
./src/server_utils.d \
./src/utils.d 

OBJS += \
./src/initial_setup.o \
./src/memoria.o \
./src/server_utils.o \
./src/utils.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

