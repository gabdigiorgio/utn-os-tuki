################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bitmap_utils.c \
../src/client_utils.c \
../src/comm_Mem.c \
../src/comm_ThreadKernel.c \
../src/file-system.c \
../src/initial_setup.c \
../src/server_utils.c \
../src/utils.c 

C_DEPS += \
./src/bitmap_utils.d \
./src/client_utils.d \
./src/comm_Mem.d \
./src/comm_ThreadKernel.d \
./src/file-system.d \
./src/initial_setup.d \
./src/server_utils.d \
./src/utils.d 

OBJS += \
./src/bitmap_utils.o \
./src/client_utils.o \
./src/comm_Mem.o \
./src/comm_ThreadKernel.o \
./src/file-system.o \
./src/initial_setup.o \
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
	-$(RM) ./src/bitmap_utils.d ./src/bitmap_utils.o ./src/client_utils.d ./src/client_utils.o ./src/comm_Mem.d ./src/comm_Mem.o ./src/comm_ThreadKernel.d ./src/comm_ThreadKernel.o ./src/file-system.d ./src/file-system.o ./src/initial_setup.d ./src/initial_setup.o ./src/server_utils.d ./src/server_utils.o ./src/utils.d ./src/utils.o

.PHONY: clean-src

