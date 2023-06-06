################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/code_reader.c \
../src/lib/exit_instruction.c \
../src/lib/instruc_log.c \
../src/lib/manejador_contexto.c \
../src/lib/set_instruction.c \
../src/lib/syscall_instruccion.c \
../src/lib/yield_instruction.c 

C_DEPS += \
./src/lib/code_reader.d \
./src/lib/exit_instruction.d \
./src/lib/instruc_log.d \
./src/lib/manejador_contexto.d \
./src/lib/set_instruction.d \
./src/lib/syscall_instruccion.d \
./src/lib/yield_instruction.d 

OBJS += \
./src/lib/code_reader.o \
./src/lib/exit_instruction.o \
./src/lib/instruc_log.o \
./src/lib/manejador_contexto.o \
./src/lib/set_instruction.o \
./src/lib/syscall_instruccion.o \
./src/lib/yield_instruction.o 


# Each subdirectory must supply rules for building sources it contributes
src/lib/%.o: ../src/lib/%.c src/lib/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-lib

clean-src-2f-lib:
	-$(RM) ./src/lib/code_reader.d ./src/lib/code_reader.o ./src/lib/exit_instruction.d ./src/lib/exit_instruction.o ./src/lib/instruc_log.d ./src/lib/instruc_log.o ./src/lib/manejador_contexto.d ./src/lib/manejador_contexto.o ./src/lib/set_instruction.d ./src/lib/set_instruction.o ./src/lib/syscall_instruccion.d ./src/lib/syscall_instruccion.o ./src/lib/yield_instruction.d ./src/lib/yield_instruction.o

.PHONY: clean-src-2f-lib

