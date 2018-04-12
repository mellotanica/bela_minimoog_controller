/***** pot.cpp *****/

#include <pot.h>
#include <math_neon.h>

pot::pot(short analogPin, float error, float minv, float maxv) :
	pin(analogPin),
	is_active(true),
	minv(minv),
	maxv(maxv),
	error(error),
	value(0)
{}
	
void pot::setup(BelaContext *context, void *userData) 
{}
	
void pot::read(BelaContext *context, void *userData, unsigned int audioFrameCount, unsigned int analogFrameCount, unsigned int digitalFrameCount) 
{
	float val = map(analogRead(context, analogFrameCount, pin), 0, 0.83, minv, maxv);
	// limit value if exceeding range
	if (val < minv) {
		val = minv;
	} else if (val > maxv) {
		val = maxv;
	}
	
	// chenge the actual value only if the difference with read data is at least the error
	if (fabsf_neon(val - value) > error) {
		value = val;
		for (pot_listener l : listeners) {
			// this could also notify another thread (or the implemented function should handle the threading?)
			l(context, value, audioFrameCount, analogFrameCount, digitalFrameCount);
		}
	}
}
	
void pot::execute(BelaContext *context, void *userData, unsigned int audioFrameCount, unsigned int analogFrameCount, unsigned int digitalFrameCount)
{}
	
void pot::cleanup(BelaContext *context, void *userData) 
{}

void pot::set_range(float mnv, float mxv)
{
	minv = mnv;
	maxv = mxv;
}

void pot::set_error(float err)
{
	error = err;
}

void pot::set_active(bool active) 
{
	is_active = active;
}

void pot::register_listener(pot_listener function)
{
	listeners.push_back(function);
}

void pot::clear_listeners()
{
	listeners.clear();
}