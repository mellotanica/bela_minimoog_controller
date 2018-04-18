/***** lfo.h *****/
#ifndef LFO_H
#define LFO_H

#include <component.h>
#include <functional>

enum lfo_shape 
{
	SQUARE = 0,
	SINE,
	TRIANGULAR,
	RAMP,
	INV_RAMP,
	SHAPE_MAX
};

class lfo: public activeComponent<float> {
public:
	lfo();
	
	void setup(BelaContext *context, void *userData);
	void read(BelaContext *context, void *userData, 
						unsigned int audioFrameCount, 
						unsigned int analogFrameCount, 
						unsigned int digitalFrameCount);
	
	void set_shape(lfo_shape shape);
	void set_frequency(float frequency);
	void set_duty_cycle(float duty_cycle);

protected:
	lfo_shape shape;
	float frequency;
	unsigned long period;
	
	float duty_cycle;
	unsigned long dc_on_samples;
	
	float phase;
	unsigned long step;
	
	float sample_rate;
	float inverse_sample_rate;
	
	std::function<float(lfo&)> eval_fun;
	
	friend float eval_square(lfo &osc);
	friend float eval_sine(lfo &osc);
	friend float eval_triangular(lfo &osc);
	friend float eval_ramp(lfo &osc);
	friend float eval_inv_ramp(lfo &osc);
};

#endif //LFO_H