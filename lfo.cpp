#include <lfo.h>
#include <math_neon.h>

inline const float eval_square(lfo &osc) 
{
	return (osc.step < osc.dc_on_samples ? 1 : 0);
}

inline const float eval_sine(lfo &osc) 
{
	return (sinf_neon(osc.phase) + 1) / 2;
}

inline const float eval_triangular(lfo &osc) 
{
	if (osc.step < osc.dc_on_samples) {
		return map(osc.step, 0, osc.dc_on_samples, 0, 1);
	} else {
		return 1 - map(osc.step, osc.dc_on_samples, osc.period, 0, 1);
	}
}

inline const float eval_ramp(lfo &osc) 
{
	return map(osc.step, 0, osc.period, 0, 1);
}

inline const float eval_inv_ramp(lfo &osc) 
{
	return 1 - map(osc.step, 0, osc.period, 0, 1);
}

lfo::lfo(std::shared_ptr<Emitter<lfo_shape>> shape):
	shape(std::make_shared<Receiver<lfo_shape>>(shape)),
	frequency(std::make_shared<Receiver<float>>()),
	duty_cycle(std::make_shared<Receiver<float>>()),
	reset_phase(std::make_shared<Receiver<bool>>()),
	value(std::make_shared<Emitter<float>>()),
	trigger(std::make_shared<Emitter<bool>>()),
	step(0),
	phase(0)
{
	value->setUpdateFunction([&](State *state)->float {
		return this->evaluate(state);
	});

	trigger->setUpdateFunction([&](State *state)->bool {
		return this->step >= this->period || this->step == 0;
	});
}

inline void lfo::update_dc(State *state, float new_freq)
{
	float new_dc = duty_cycle->getValue(state);
	if((new_dc != prev_dc || new_freq != prev_freq) && new_dc >= 0 && new_dc <= 1)
	{
		dc_on_samples = (unsigned long) ((float) period * new_dc);
		prev_dc = new_dc;
	}
}

float lfo::evaluate(State *state) 
{
	float new_freq = frequency->getValue(state);
	if(new_freq > 0 && new_freq != prev_freq){
		period = (unsigned long) ((float) state->context->audioSampleRate / frequency->getValue(state));
	}

	if(reset_phase->getValue(state)){
		step = phase = 0;
	} else {
		if(step >= period) {
			step = 0;
		}
		if(phase > 2.0 * M_PI) {
			phase -= 2.0 * M_PI;
		}
	}

	float val;
	switch(shape->getValue(state)) {
		case SQUARE:
			update_dc(state, new_freq);
			val = eval_square(*this);
			break;
			
		case SINE:
			val = eval_sine(*this);
			break;
		
		case TRIANGULAR:
			update_dc(state, new_freq);
			val = eval_triangular(*this);
			break;
		
		case RAMP:
			val = eval_ramp(*this);
			break;
		
		case INV_RAMP:
			val = eval_inv_ramp(*this);
			break;
	}

	prev_freq = new_freq;
	step ++;

	phase += 2.0 * M_PI * new_freq * state->inverse_sample_rate;

	return val;
}

