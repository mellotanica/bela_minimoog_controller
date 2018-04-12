/***** led.cpp *****/

#include <led.h>

led::led(short pin) :
	pin(pin),
	is_active(false),
	state(false),
	frames_count(0)
{}
	
void led::setup(BelaContext *context, void *userData) 
{
	pinMode(context, 0, pin, OUTPUT);
}
	
void led::read(BelaContext *context, void *userData, unsigned int audioFrameCount, unsigned int analogFrameCount, unsigned int digitalFrameCount) 
{}
	
void led::execute(BelaContext *context, void *userData, unsigned int audioFrameCount, unsigned int analogFrameCount, unsigned int digitalFrameCount)
{
	if(is_active){
		if (frames_count >= pwm_duration) {
			set_state(context, digitalFrameCount, false);
			frames_count = 0;
		} else if (frames_count >= pwm_duty_cycle_duration && !state) {
			set_state(context, digitalFrameCount, true);
		}
		frames_count ++;
	} else if(state) {
		set_state(context, digitalFrameCount, false);
	}
}
	
void led::cleanup(BelaContext *context, void *userData) 
{}

void led::set_active(bool active)
{
	is_active = active;
	if (active) {
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
	pwm_duration = (int) (context->analogSampleRate * pwm_period);
	pwm_duty_cycle_duration = (int) ((float)pwm_duration * pwm_duty_cycle);
}
