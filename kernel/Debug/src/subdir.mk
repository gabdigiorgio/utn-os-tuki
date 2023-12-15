################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/client_utils.c \
../src/comm_threadKernel.c \
../src/console_threads.c \
../src/initial_setup.c \
../src/kernel.c \
../src/long_term_planification.c \
../src/server_utils.c \
../src/short_term_planification.c \
../src/utils.c 

C_DEPS += \
./src/client_utils.d \
./src/comm_threadKernel.d \
./src/console_threads.d \
./src/initial_setup.d \
./src/kernel.d \
./src/long_term_planification.d \
./src/server_utils.d \
./src/short_term_planification.d \
./src/utils.d 

OBJS += \
./src/client_utils.o \
./src/comm_threadKernel.o \
./src/console_threads.o \
./src/initial_setup.o \
./src/kernel.o \
./src/long_term_planification.o \
./src/server_utils.o \
./src/short_term_planification.o \
./src/utils.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/client_utils.d ./src/client_utils.o ./src/comm_threadKernel.d ./src/comm_threadKernel.o ./src/console_threads.d ./src/console_threads.o ./src/initial_setup.d ./src/initial_setup.o ./src/kernel.d ./src/kernel.o ./src/long_term_planification.d ./src/long_term_planification.o ./src/server_utils.d ./src/server_utils.o ./src/short_term_planification.d ./src/short_term_planification.o ./src/utils.d ./src/utils.o

.PHONY: clean-src

