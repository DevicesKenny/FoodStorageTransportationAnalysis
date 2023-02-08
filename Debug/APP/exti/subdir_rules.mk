################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
APP/exti/%.obj: ../APP/exti/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ccs/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ccs/DSP8233x_Project/DSP8233x_Example/Produce_Transport" --include_path="D:/ccs/DSP8233x_Project/DSP8233x_Example/Produce_Transport/APP/wifi" --include_path="D:/ccs/DSP8233x_Project/DSP8233x_Example/Produce_Transport/APP/exti" --include_path="D:/ccs/DSP8233x_Project/DSP8233x_Example/Produce_Transport/APP/relay" --include_path="D:/ccs/DSP8233x_Project/DSP8233x_Example/Produce_Transport/APP/beep" --include_path="D:/ccs/DSP8233x_Project/DSP8233x_Example/Produce_Transport/APP/lcd9648" --include_path="D:/ccs/DSP8233x_Project/DSP8233x_Example/Produce_Transport/APP/sht30" --include_path="D:/ccs/DSP8233x_Project/DSP8233x_Example/Produce_Transport/APP/24cxx" --include_path="D:/ccs/DSP8233x_Project/DSP8233x_Example/Produce_Transport/APP/iic" --include_path="D:/ccs/DSP8233x_Project/DSP8233x_Example/Produce_Transport/APP/adc" --include_path="D:/ccs/DSP8233x_Project/DSP8233x_Example/Produce_Transport/APP/key" --include_path="D:/ccs/DSP8233x_Project/DSP8233x_Example/Produce_Transport/APP/DC_Motor" --include_path="D:/ccs/DSP8233x_Project/DSP8233x_Example/Produce_Transport/APP/epwm" --include_path="D:/ccs/DSP8233x_Project/DSP8233x_Example/Produce_Transport/APP/leds" --include_path="D:/ccs/DSP8233x_Project/DSP8233x_Example/Produce_Transport/APP/smg" --include_path="D:/ccs/DSP8233x_Project/DSP8233x_Example/Produce_Transport/APP/time" --include_path="D:/ccs/DSP8233x_ProjectExample/DSP2833x_Libraries/DSP2833x_common/include" --include_path="D:/ccs/DSP8233x_ProjectExample/DSP2833x_Libraries/DSP2833x_headers/include" --include_path="D:/ccs/DSP8233x_Project/DSP8233x_Example/Produce_Transport/APP/uart" --include_path="D:/ccs/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="APP/exti/$(basename $(<F)).d_raw" --obj_directory="APP/exti" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


