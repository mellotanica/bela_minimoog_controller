#ifndef SMOOTHER_H
#define SMOOTHER_H

#include <base/component.h>

#include <components/adsr.h>
#include <components/comparator.h>
#include <components/mixer.h>
#include <components/freeze.h>

static auto smoother_envelope_time = constant<float>::make(0.01);

static auto smoother_input_threshold = constant<float>::make(0.02);

class smoother : public component {
public:
	smoother(EmitterP<float> input = nullptr,
			EmitterP<float> input_threshold = smoother_input_threshold):
		env(smoother_envelope_time, 
			ZeroF, 
			OneF, 
			smoother_envelope_time, 
			OneF, 
			True, 
			adsr_logaritmic,
			adsr_linear,
			adsr_exponential),
		gate(high_pass_exclusive_comparator,
			input,
			input_threshold),
		mix(mixer_mul),
		release_freeze()
	{
		this->input = gate.input;
		
		this->input_threshold = gate.threshold_a;

		env.gate->register_emitter(gate.output);

		release_freeze.input->register_emitter(this->input);
		release_freeze.engage->register_emitter(this->gate.outputNeg);

		mix.register_input(release_freeze.output);
		mix.register_input(env.value);

		this->output = mix.output;
	};

	ReceiverP <float> input;
	ReceiverP <float> input_threshold;

	EmitterP <float> output;
private:
	adsr env;
	comparator <float> gate;
	mixer <float> mix;
	freeze <float> release_freeze;
};

#endif //SMOOTHER_H
