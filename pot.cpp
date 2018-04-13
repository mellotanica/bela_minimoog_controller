/***** pot.cpp *****/

#include <pot.h>
#include <math_neon.h>

pot::pot(short analogPin, float error, float minv, float maxv) :
	pin(analogPin),
	error(error),
	value(0)
{
	set_range(minv, maxv);
}

void pot::read(BelaContext *context, void *userData, unsigned int audioFrameCount, unsigned int analogFrameCount, unsigned int digitalFrameCount) 
{
	float val = map(analogRead(context, analogFrameCount, pin), 0, 0.83, minv, maxv);
	// limit value if exceeding range
	if (val < minv) {
		val = minv;
	} else if (val > maxv) {
		val = maxv;
	}
	
	if (inverse_reading) {
		val = maxv - val;
	}
	
	// chenge the actual value only if the difference with read data is at least the error
	if (fabsf_neon(val - value) > error) {
		value = val;
		updateValue(context, value, audioFrameCount, analogFrameCount, digitalFrameCount);
	}
}

void pot::set_range(float mnv, float mxv)
{
	if(mxv > mnv) {
		inverse_reading = true;
		minv = mnv;
		maxv = mxv;
	} else {
		inverse_reading = false;
		minv = mxv;
		maxv = mnv;
	}
}

void pot::set_error(float err)
{
	error = err;
}
