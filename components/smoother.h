#ifndef SMOOTHER_H
#define SMOOTHER_H

#include <base/component.h>

#include <components/adsr.h>
#include <components/comparator.h>
#include <components/mixer.h>

static auto smoother_envelope_time = constant<float>::make(0.01);

class smoother : public component {
public:
	smoother(EmitterP<float> input = nullptr):
		env(smoother_envelope_time, 
			ZeroF, 
			OneF, 
			smoother_envelope_time, 
			OneF, 
			False, 
			adsr_logaritmic_increment,
			adsr_linear_decrement,
			adsr_logaritmic_decrement),
		gate(high_pass_exclusive_comparator,
			input),
		mix(mixer_mul)
	{
		this->input = gate.input;
		this->output = mix.output;

		env.gate->register_emitter(gate.output);

		mix.register_input(gate.input);
		mix.register_input(env.value);
	};

	ReceiverP <float> input;

	EmitterP <float> output;
private:
	adsr env;
	comparator <float> gate;
	mixer <float> mix;
};

#endif //SMOOTHER_H
