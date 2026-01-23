################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ayudas.c \
../src/cat.c \
../src/cd.c \
../src/clear.c \
../src/cp.c \
../src/echo.c \
../src/grep.c \
../src/logs.c \
../src/ls.c \
../src/mkdir.c \
../src/pwd.c \
../src/rm.c \
../src/shell.c 

C_DEPS += \
./src/ayudas.d \
./src/cat.d \
./src/cd.d \
./src/clear.d \
./src/cp.d \
./src/echo.d \
./src/grep.d \
./src/logs.d \
./src/ls.d \
./src/mkdir.d \
./src/pwd.d \
./src/rm.d \
./src/shell.d 

OBJS += \
./src/ayudas.o \
./src/cat.o \
./src/cd.o \
./src/clear.o \
./src/cp.o \
./src/echo.o \
./src/grep.o \
./src/logs.o \
./src/ls.o \
./src/mkdir.o \
./src/pwd.o \
./src/rm.o \
./src/shell.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/ayudas.d ./src/ayudas.o ./src/cat.d ./src/cat.o ./src/cd.d ./src/cd.o ./src/clear.d ./src/clear.o ./src/cp.d ./src/cp.o ./src/echo.d ./src/echo.o ./src/grep.d ./src/grep.o ./src/logs.d ./src/logs.o ./src/ls.d ./src/ls.o ./src/mkdir.d ./src/mkdir.o ./src/pwd.d ./src/pwd.o ./src/rm.d ./src/rm.o ./src/shell.d ./src/shell.o

.PHONY: clean-src

