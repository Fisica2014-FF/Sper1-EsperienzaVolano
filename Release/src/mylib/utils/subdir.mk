################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/mylib/utils/NomeDiFile.cpp 

OBJS += \
./src/mylib/utils/NomeDiFile.o 

CPP_DEPS += \
./src/mylib/utils/NomeDiFile.d 


# Each subdirectory must supply rules for building sources it contributes
src/mylib/utils/%.o: ../src/mylib/utils/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


