#ifndef LFO_H
#define LFO_H

#include <base/component.h>

#include <components/constant.h>

enum lfo_shape 
{
	SQUARE = 0,
	SINE,
	TRIANGULAR,
	RAMP,
	INV_RAMP,
};

static auto square_lfo = constant<lfo_shape>::make(SQUARE);
static auto sine_lfo = constant<lfo_shape>::make(SINE);
static auto triangular_lfo = constant<lfo_shape>::make(TRIANGULAR);
static auto ramp_lfo = constant<lfo_shape>::make(RAMP);
static auto inv_ramp_lfo = constant<lfo_shape>::make(INV_RAMP);

static auto lfo_shape_maxF = constant<float>::make(INV_RAMP+1);

class lfo: public component {
public:
	lfo(EmitterP<lfo_shape> shape = square_lfo);
	
	ReceiverP<lfo_shape> shape;
	ReceiverP<float> frequency;
	ReceiverP<float> duty_cycle;
	ReceiverP<bool> reset_phase;

	EmitterP<float> value;
	EmitterP<bool> trigger;
protected:
	unsigned long period;
	unsigned long dc_on_samples;

	unsigned long step;
	float phase;
	
	friend inline const float __lfo_eval_square(lfo &osc);
	friend inline const float __lfo_eval_sine(lfo &osc);
	friend inline const float __lfo_eval_triangular(lfo &osc);
	friend inline const float __lfo_eval_ramp(lfo &osc);
	friend inline const float __lfo_eval_inv_ramp(lfo &osc);
};


#endif //LFO_H
