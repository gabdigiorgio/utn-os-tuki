################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/client_utils.c \
../src/consola.c \
../src/initial_setup.c \
../src/utils.c 

C_DEPS += \
./src/client_utils.d \
./src/consola.d \
./src/initial_setup.d \
./src/utils.d 

OBJS += \
./src/client_utils.o \
./src/consola.o \
./src/initial_setup.o \
./src/utils.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



