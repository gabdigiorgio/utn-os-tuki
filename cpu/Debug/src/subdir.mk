################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/client_utils.c \
../src/comm_thread.c \
../src/cpu.c \
../src/initial_setup.c \
../src/mmu.c \
../src/server_utils.c \
../src/utils.c 

C_DEPS += \
./src/client_utils.d \
./src/comm_thread.d \
./src/cpu.d \
./src/initial_setup.d \
./src/mmu.d \
./src/server_utils.d \
./src/utils.d 

OBJS += \
./src/client_utils.o \
./src/comm_thread.o \
./src/cpu.o \
./src/initial_setup.o \
./src/mmu.o \
./src/server_utils.o \
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
	-$(RM) ./src/client_utils.d ./src/client_utils.o ./src/comm_thread.d ./src/comm_thread.o ./src/cpu.d ./src/cpu.o ./src/initial_setup.d ./src/initial_setup.o ./src/mmu.d ./src/mmu.o ./src/server_utils.d ./src/server_utils.o ./src/utils.d ./src/utils.o

.PHONY: clean-src

