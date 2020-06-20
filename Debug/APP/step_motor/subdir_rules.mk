################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
APP/step_motor/step_motor.obj: ../APP/step_motor/step_motor.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"E:/software/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="E:/DSP8233x_ProjectExample/work/smart fans/APP/IICOLED" --include_path="E:/DSP8233x_ProjectExample/work/smart fans/APP/dc_motor" --include_path="E:/DSP8233x_ProjectExample/work/smart fans/APP/Public" --include_path="E:/DSP8233x_ProjectExample/work/smart fans/APP" --include_path="E:/DSP8233x_ProjectExample/work/smart fans/APP/epwm" --include_path="E:/DSP8233x_ProjectExample/DSP2833x_Libraries/DSP2833x_common/include" --include_path="E:/DSP8233x_ProjectExample/work/smart fans/Debug/DSP2833x_Libraries" --include_path="E:/DSP8233x_ProjectExample/work/smart fans/DSP2833x_Libraries" --include_path="E:/DSP8233x_ProjectExample/DSP2833x_Libraries/DSP2833x_headers/include" --include_path="E:/DSP8233x_ProjectExample/work/smart fans/APP/beep" --include_path="E:/DSP8233x_ProjectExample/work/smart fans/APP/exti" --include_path="E:/DSP8233x_ProjectExample/work/smart fans/APP/key" --include_path="E:/DSP8233x_ProjectExample/work/smart fans/APP/lcd1602" --include_path="E:/DSP8233x_ProjectExample/work/smart fans/APP/lcd9648" --include_path="E:/DSP8233x_ProjectExample/work/smart fans/APP/leds" --include_path="E:/DSP8233x_ProjectExample/work/smart fans/APP/relay" --include_path="E:/DSP8233x_ProjectExample/work/smart fans/APP/smg" --include_path="E:/DSP8233x_ProjectExample/work/smart fans/APP/step_motor" --include_path="E:/DSP8233x_ProjectExample/work/smart fans/APP/time" --include_path="E:/DSP8233x_ProjectExample/work/smart fans/APP/uart" --include_path="E:/software/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/include" -g --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="APP/step_motor/step_motor.d" --obj_directory="APP/step_motor" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


