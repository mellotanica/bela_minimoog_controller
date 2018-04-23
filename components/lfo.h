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

class lfo: public component {
public:
	lfo(std::shared_ptr<Emitter<lfo_shape>> shape = square_lfo);
	
	std::shared_ptr<Receiver<lfo_shape>> shape;
	std::shared_ptr<Receiver<float>> frequency;
	std::shared_ptr<Receiver<float>> duty_cycle;
	std::shared_ptr<Receiver<bool>> reset_phase;

	std::shared_ptr<Emitter<float>> value;
	std::shared_ptr<Emitter<bool>> trigger;
protected:
	unsigned long period;
	unsigned long dc_on_samples;

	float prev_freq;
	float prev_dc;
	
	unsigned long step;
	float phase;
	
	float evaluate(State *state);
	
	inline void update_dc(State *state, float new_freq);

	friend inline const float eval_square(lfo &osc);
	friend inline const float eval_sine(lfo &osc);
	friend inline const float eval_triangular(lfo &osc);
	friend inline const float eval_ramp(lfo &osc);
	friend inline const float eval_inv_ramp(lfo &osc);
};


#endif //LFO_H
