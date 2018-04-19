/***** led.cpp *****/

#include <led.h>

led::led(short pin) :
	pin(pin),
	frames_count(0)
{}
	
void led::setup(BelaContext *context, void *userData) 
{
	pinMode(context, 0, pin, OUTPUT);
}
	
void led::render(State *execState)
{
	if(state.getValue(execState)) {
		fixup_pwm_durations(execState);
		if (pwm_enabled) {
			if (frames_count >= pwm_duration) {
				set_state(execState, true);
				frames_count = 0;
			} else if (frames_count >= pwm_duty_cycle_duration) {
				set_state(execState, false);
			}
			frames_count ++;
		} else {
			set_state(execState, pwm_duty_cycle.getValue(execState) > 0.5);
		}
	} else {
		set_state(execState, false);
	}
}

void led::set_state(State *execState, bool st)
{
	digitalWrite(execState->context, 
			execState->digitalFrame, 
			pin, 
			(st ? GPIO_HIGH : GPIO_LOW));
}

void led::fixup_pwm_durations(State *execState)
{
	float dc = pwm_duty_cycle.getValue(execState);
	if((dc - PWM_DC_ERROR) <= 0) {
		pwm_enabled = false;
	} else if((dc + PWM_DC_ERROR) >= 1) {
		pwm_enabled = false;
	} else {
		pwm_enabled = true;
		pwm_duration = (int) (execState->context->analogSampleRate * pwm_period.getValue(execState));
		pwm_duty_cycle_duration = (int) ((float)pwm_duration * dc);
	}
}
