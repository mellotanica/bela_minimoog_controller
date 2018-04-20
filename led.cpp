/***** led.cpp *****/

#include <led.h>

led::led(short pin) :
	state(std::make_shared<Receiver<bool>>()),
	pwm_period(std::make_shared<Receiver<float>>()),
	pwm_duty_cycle(std::make_shared<Receiver<float>>()),
	pin(pin),
	frames_count(0)
{}
	
void led::setup(BelaContext *context, void *userData) 
{
	pinMode(context, 0, pin, OUTPUT);
}
	
void led::render(State *execState)
{
	if(state->getValue(execState)) {
		bool no_pwm_state = fixup_pwm_durations(execState);
		if (pwm_enabled) {
			if (frames_count >= pwm_duration) {
				set_state(execState, true);
				frames_count = 0;
			} else if (frames_count >= pwm_duty_cycle_duration) {
				set_state(execState, false);
			}
			frames_count ++;
		} else {
			set_state(execState, no_pwm_state);
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

bool led::fixup_pwm_durations(State *execState)
{
	float dc = pwm_duty_cycle->getValue(execState);
	bool pwm_off_state = false;
	if((dc - PWM_DC_ERROR) <= 0) {
		pwm_enabled = false;
	} else if((dc + PWM_DC_ERROR) >= 1) {
		pwm_enabled = false;
		pwm_off_state = true;
	} else {
		pwm_enabled = true;
		pwm_duration = (int) (execState->context->analogSampleRate * pwm_period->getValue(execState));
		pwm_duty_cycle_duration = (int) ((float)pwm_duration * dc);
	}
	return pwm_off_state;
}
