################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cliente_utils.c \
../src/file-system.c \
../src/initial_setup.c \
../src/server_utils.c \
../src/utils.c 

C_DEPS += \
./src/cliente_utils.d \
./src/file-system.d \
./src/initial_setup.d \
./src/server_utils.d \
./src/utils.d 

OBJS += \
./src/cliente_utils.o \
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
	-$(RM) ./src/cliente_utils.d ./src/cliente_utils.o ./src/file-system.d ./src/file-system.o ./src/initial_setup.d ./src/initial_setup.o ./src/server_utils.d ./src/server_utils.o ./src/utils.d ./src/utils.o

.PHONY: clean-src

