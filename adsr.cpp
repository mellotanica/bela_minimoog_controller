#include <components/adsr.h>
#include <math_neon.h>

#define DECAY_ERROR 0.0005f

adsr::adsr(EmitterP<float> attack,
		EmitterP<float> decay,
		EmitterP<float> sustain,
		EmitterP<float> release,
		EmitterP<float> attack_level,
		EmitterP<bool> soft_reset,
		EmitterP<adsr_shape_fun> attack_function,
		EmitterP<adsr_shape_fun> decay_function,
		EmitterP<adsr_shape_fun> release_function):
	attack(Receiver<float>::make(attack)),
	decay(Receiver<float>::make(decay)),
	sustain(Receiver<float>::make(sustain)),
	release(Receiver<float>::make(release)),
	attack_level(Receiver<float>::make(attack_level)),
	gate(Receiver<bool>::make(False)),
	soft_reset(Receiver<bool>::make(soft_reset)),
	attack_function(Receiver<adsr_shape_fun>::make(attack_function)),
	decay_function(Receiver<adsr_shape_fun>::make(decay_function)),
	release_function(Receiver<adsr_shape_fun>::make(release_function)),
	value(Emitter<float>::make()),
	inactive(Emitter<bool>::make()),
	active_state(IDLE)
{
	inactive->setUpdateFunction([&](State *state)->bool {
		return this->active_state == IDLE;
	});

	value->setUpdateFunction([&](State *state)->float {
		return this->evaluate(state);
	});
}

void adsr::reset()
{
	attack->register_emitter(adsr_default_time);
	decay->register_emitter(adsr_default_time);
	release->register_emitter(adsr_default_time);
	sustain->register_emitter(adsr_default_sustain);
	gate->register_emitter(False);
	soft_reset->register_emitter(True);
	attack_level->register_emitter(OneF);
	attack_function->register_emitter(adsr_linear);
	decay_function->register_emitter(adsr_linear);
	release_function->register_emitter(adsr_linear);
}

float adsr::evaluate(State *state)
{
	float time = step * state->inverse_sample_rate;

	bool gate_trigger = last_gate != gate->getValue(state);
	last_gate = gate->getValue(state);

	step ++;

	if(gate_trigger){
		switch(active_state) {
			case IDLE:
			case RELEASE:
				active_state = ATTACK;
				step = 0;
				phaseStart = lastV * soft_reset->getValue(state);
				break;
			default:
				active_state = RELEASE;
				step = 0;
				phaseStart = lastV;
				break;
		}
	} else {
		switch(active_state) {
			case ATTACK:
				if(time > attack->getValue(state)) {
					if(decay->getValue(state) - DECAY_ERROR <= 0)
					{
						active_state = SUSTAIN;
					} else {
						active_state = DECAY;
						phaseStart = lastV;
					}
					step = 0;
				}
				break;
			case DECAY:
				if(time > decay->getValue(state)) {
					active_state = SUSTAIN;
				}
				break;
			case RELEASE:
				if(time > release->getValue(state)) {
					active_state = IDLE;
				}
				break;
			default:
				break;
		}
	}

	switch(active_state) {
		case ATTACK:
			lastV = attack_function->getValue(state)(time/attack->getValue(state), phaseStart, attack_level->getValue(state));
			break;
		case DECAY:
			lastV = decay_function->getValue(state)(time/decay->getValue(state), phaseStart, sustain->getValue(state));
			break;
		case SUSTAIN:
			lastV = sustain->getValue(state);
			break;
		case RELEASE:
			lastV = release_function->getValue(state)(time/release->getValue(state), phaseStart, 0);
			break;
		default:
			lastV = 0;
			break;
	}
	return lastV;
}

float __adsr_linear(float phase, float start, float target)
{
	if(start < target) {
		return start + (fabsf_neon(target - start) * phase);
	} else {
		return start - (fabsf_neon(target - start) * phase);
	}
}

float __adsr_logaritmic(float phase, float start, float target)
{
	if(start < target) {
		return start + (fabsf_neon(target - start) * sinf_neon(phase*M_PI/2));
	} else {
		return target + (fabsf_neon(target - start) * cosf_neon(phase*M_PI/2));
	}
}

float __adsr_exponential(float phase, float start, float target)
{
	if(start < target) {
		return start + (fabsf_neon(target - start) * (1 + cosf_neon(M_PI + (phase*M_PI/2))));
	} else {
		return target + (fabsf_neon(target - start) * (1 + sinf_neon(M_PI + (phase*M_PI/2))));
	}
}
