################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/algoritmos.c \
../src/client_utils.c \
../src/comm_threadMem.c \
../src/comm_thread_cpu.c \
../src/initial_setup.c \
../src/memoria.c \
../src/segmentacion.c \
../src/server_utils.c \
../src/utils.c 

C_DEPS += \
./src/algoritmos.d \
./src/client_utils.d \
./src/comm_threadMem.d \
./src/comm_thread_cpu.d \
./src/initial_setup.d \
./src/memoria.d \
./src/segmentacion.d \
./src/server_utils.d \
./src/utils.d 

OBJS += \
./src/algoritmos.o \
./src/client_utils.o \
./src/comm_threadMem.o \
./src/comm_thread_cpu.o \
./src/initial_setup.o \
./src/memoria.o \
./src/segmentacion.o \
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
	-$(RM) ./src/algoritmos.d ./src/algoritmos.o ./src/client_utils.d ./src/client_utils.o ./src/comm_threadMem.d ./src/comm_threadMem.o ./src/comm_thread_cpu.d ./src/comm_thread_cpu.o ./src/initial_setup.d ./src/initial_setup.o ./src/memoria.d ./src/memoria.o ./src/segmentacion.d ./src/segmentacion.o ./src/server_utils.d ./src/server_utils.o ./src/utils.d ./src/utils.o

.PHONY: clean-src

