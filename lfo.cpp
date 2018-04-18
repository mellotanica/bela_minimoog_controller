/***** lfo.cpp *****/
#include <lfo.h>
#include <math_neon.h>
	
float eval_square(lfo &osc) {
	return (osc.step < osc.dc_on_samples ? 1 : 0);
}

float eval_sine(lfo &osc) {
	return (sinf_neon(osc.phase) + 1) / 2;
}

float eval_triangular(lfo &osc) {
	if (osc.step < osc.dc_on_samples) {
		return map(osc.step, 0, osc.dc_on_samples, 0, 1);
	} else {
		return 1 - map(osc.step, osc.dc_on_samples, osc.period, 0, 1);
	}
}

float eval_ramp(lfo &osc) {
	return map(osc.step, 0, osc.period, 0, 1);
}

float eval_inv_ramp(lfo &osc) {
	return 1 - map(osc.step, 0, osc.period, 0, 1);
}

lfo::lfo():
	frequency(10),
	duty_cycle(0.5f),
	phase(0),
	step(0)
{
	set_shape(SQUARE);
}

void lfo::setup(BelaContext *context, void *userData)
{
	sample_rate = context->audioSampleRate;
	inverse_sample_rate = 1.0 / sample_rate;
	// set frequency to calculate period
	set_frequency(frequency);
}

void lfo::read(BelaContext *context, void *userData, 
						unsigned int audioFrameCount, 
						unsigned int analogFrameCount, 
						unsigned int digitalFrameCount)
{
	if (step >= period) {
		step = 0;
	}
	if(phase > 2.0 * M_PI) {
		phase -= 2.0 * M_PI;
	}
	float value = eval_fun(*this);
	updateValue(context, value, audioFrameCount, analogFrameCount, digitalFrameCount);
	step ++;
	phase += 2.0 * M_PI * frequency * inverse_sample_rate;
}
	
void lfo::set_shape(lfo_shape s)
{
	shape = s;
	switch(shape) {
		case SQUARE:
			eval_fun = eval_square;
			break;
			
		case SINE:
			eval_fun = eval_sine;
			break;
		
		case TRIANGULAR:
			eval_fun = eval_triangular;
			break;
		
		case RAMP:
			eval_fun = eval_ramp;
			break;
		
		case INV_RAMP:
			eval_fun = eval_inv_ramp;
			break;
			
		case SHAPE_MAX:
			eval_fun = eval_square;
			break;
	}
}

void lfo::set_frequency(float freq)
{
	if(frequency >= 0) 
	{
		frequency = freq;
		period = (unsigned long) (sample_rate / frequency);
		dc_on_samples = (unsigned long) (period * duty_cycle);
	}
}

void lfo::set_duty_cycle(float dc) 
{
	if (dc >= 0 && dc <= 1) 
	{
		duty_cycle = dc;
		dc_on_samples = (unsigned long) (period * duty_cycle);
	}
}