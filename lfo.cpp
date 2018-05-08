#include <components/lfo.h>

#include <components/led.h>

#include <math_neon.h>

#define PHASE_3_2_PI 1.5 * M_PI
#define PHASE_ERROR 0.02 // approx. 1 angular degree

inline const float __lfo_eval_square(lfo &osc) 
{
	return ((osc.duty_cycle->getLastValue() + PWM_DC_ERROR) >= 1 ||
			osc.step < osc.dc_on_samples ? 1 : 0);
}

inline const float __lfo_eval_sine(lfo &osc) 
{
	return (sinf_neon(osc.phase) + 1) / 2;
}

inline const float __lfo_eval_triangular(lfo &osc) 
{
	if (osc.step < osc.dc_on_samples) {
		return map(osc.step, 0, osc.dc_on_samples, 0, 1);
	} else {
		return 1 - map(osc.step, osc.dc_on_samples, osc.period, 0, 1);
	}
}

inline const float __lfo_eval_ramp(lfo &osc) 
{
	return map(osc.step, 0, osc.period, 0, 1);
}

inline const float __lfo_eval_inv_ramp(lfo &osc) 
{
	return 1 - map(osc.step, 0, osc.period, 0, 1);
}

lfo::lfo(EmitterP<lfo_shape> shape):
	shape(Receiver<lfo_shape>::make(shape)),
	frequency(Receiver<float>::make()),
	duty_cycle(Receiver<float>::make()),
	reset_phase(Receiver<bool>::make()),
	value(Emitter<float>::make()),
	trigger(Emitter<bool>::make()),
	step(0),
	phase(0)
{
	value->setUpdateFunction([&](State *state)->float {
		this->period = (unsigned long) (
				(float) state->context->audioSampleRate / 
				this->frequency->getValue(state));

		if(this->reset_phase->getValue(state)){
			this->step = 0;
			this->phase = PHASE_3_2_PI;
		} else {
			if(this->step >= this->period) {
				this->step = 0;
			}
			if(this->phase > 2.0 * M_PI) {
				this->phase -= 2.0 * M_PI;
			}
		}

		float val;
		switch(this->shape->getValue(state)) {
			case SQUARE:
				this->dc_on_samples = (unsigned long) (
						(float) this->period *
						this->duty_cycle->getValue(state));
				val = __lfo_eval_square(*this);
				break;
				
			case SINE:
				val = __lfo_eval_sine(*this);
				break;
			
			case TRIANGULAR:
				this->dc_on_samples = (unsigned long) (
						(float) this->period *
						this->duty_cycle->getValue(state));
				val = __lfo_eval_triangular(*this);
				break;
			
			case RAMP:
				val = __lfo_eval_ramp(*this);
				break;
			
			case INV_RAMP:
				val = __lfo_eval_inv_ramp(*this);
				break;
		}

		this->step ++;

		this->phase += 2.0 * M_PI * this->frequency->getValue(state) * 
			state->inverse_sample_rate;

		return val;
	});

	trigger->setUpdateFunction([&](State *state)->bool {
		switch(this->shape->getValue(state)) {
		case SINE:
			return fabsf_neon(this->phase - PHASE_3_2_PI) <= PHASE_ERROR;
		default:
			return (this->step >= this->period || this->step == 0);
		}
	});
}

