################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/code_reader.c \
../src/lib/instruc_log.c \
../src/lib/manejador_contexto.c 

C_DEPS += \
./src/lib/code_reader.d \
./src/lib/instruc_log.d \
./src/lib/manejador_contexto.d 

OBJS += \
./src/lib/code_reader.o \
./src/lib/instruc_log.o \
./src/lib/manejador_contexto.o 


# Each subdirectory must supply rules for building sources it contributes
src/lib/%.o: ../src/lib/%.c src/lib/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-lib

clean-src-2f-lib:
	-$(RM) ./src/lib/code_reader.d ./src/lib/code_reader.o ./src/lib/instruc_log.d ./src/lib/instruc_log.o ./src/lib/manejador_contexto.d ./src/lib/manejador_contexto.o

.PHONY: clean-src-2f-lib

