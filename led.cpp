/***** led.cpp *****/

#include <led.h>

led::led(short pin, float pwm_period, float pwm_duty_cycle) :
	pin(pin),
	state(false),
	pwm_period(pwm_period),
	pwm_duty_cycle(pwm_duty_cycle),
	frames_count(0)
{
	required_state = active_state;
}
	
void led::setup(BelaContext *context, void *userData) 
{
	pinMode(context, 0, pin, OUTPUT);
	fixup_pwm_durations(context);
}
	
void led::execute(BelaContext *context, void *userData, unsigned int audioFrameCount, unsigned int analogFrameCount, unsigned int digitalFrameCount)
{
	if (pwm_enabled) {
		if (frames_count >= pwm_duration) {
			set_state(context, digitalFrameCount, true);
			frames_count = 0;
		} else if (frames_count >= pwm_duty_cycle_duration) {
			set_state(context, digitalFrameCount, false);
		}
		frames_count ++;
	} else {
		set_state(context, digitalFrameCount, pwm_duty_cycle > 0);
	}
	if (required_state != active_state) {
		rt_printf("execute, req_st: %d, act_st: %d\n", required_state, active_state);
		if (!required_state) {
			set_state(context, digitalFrameCount, false);
		}
		active_state = required_state;
	}
}

void led::set_active(bool active)
{
	// the function is overridden to let the execute be called at least one other time to turn off the led
	required_state = active;
	if (active) {
		active_state = required_state;
		frames_count = 0;
	}
}

void led::set_state(BelaContext *context, int reference_frame, bool st)
{
	if (state != st) {
		state = st;
		digitalWrite(context, reference_frame, pin, (state ? GPIO_HIGH : GPIO_LOW));
	}
}

void led::set_pwm_period(BelaContext *context, float period)
{
	if (pwm_period != period) {
		pwm_period = period;
		fixup_pwm_durations(context);
	}
}
	
void led::set_pwm_duty_cycle(BelaContext *context, float dc)
{
	if (pwm_duty_cycle != dc) {
		pwm_duty_cycle = dc;
		fixup_pwm_durations(context);
	}
}

void led::fixup_pwm_durations(BelaContext *context)
{
	if((pwm_duty_cycle - PWM_DC_ERROR) <= 0) {
		pwm_enabled = false;
		pwm_duty_cycle = 0;
	} else if((pwm_duty_cycle + PWM_DC_ERROR) >= 1) {
		pwm_enabled = false;
		pwm_duty_cycle = 1;
	} else {
		pwm_enabled = true;
		pwm_duration = (int) (context->analogSampleRate * pwm_period);
		pwm_duty_cycle_duration = (int) ((float)pwm_duration * pwm_duty_cycle);
	}
}
