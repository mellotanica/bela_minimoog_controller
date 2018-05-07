#ifndef ADSR_TEST_H
#define ADSR_TEST_H

#include <base/program.h>

#include <components/adsr.h>
#include <components/mixer.h>
#include <components/converter.h>

class adsr_test: public program {
public:
	void load_program()
	{
		hardware& hw = hardware::getInstance();
		
		int i;

		for (i = 0; i < 3; ++i) {
			hw.pots[i]->minv->register_emitter(adsr_default_mintime);
			hw.pots[i]->maxv->register_emitter(adsr_default_maxtime);
		}

		gate_mix.mix_mode->register_emitter(mixer_or);
		gate_mix.register_input(hw.killswitches[0]->gate);
		gate_mix.register_input(hw.midi->gate.value);

		for (i = 0; i < 4; ++i) {
			env_gens[i].attack->register_emitter(hw.pots[0]->value);
			env_gens[i].decay->register_emitter(hw.pots[1]->value);
			env_gens[i].release->register_emitter(hw.pots[2]->value);

			env_gens[i].attack_level->register_emitter(hw.pots[3]->value);
			env_gens[i].sustain->register_emitter(hw.pots[4]->value);

			env_gens[i].gate->register_emitter(gate_mix.output);
		}

		env_gens[1].attack_function->register_emitter(adsr_logaritmic_increment);
		env_gens[1].decay_function->register_emitter(adsr_logaritmic_decrement);
		env_gens[1].release_function->register_emitter(adsr_logaritmic_decrement);
		
		env_gens[2].attack_function->register_emitter(adsr_exponential_increment);
		env_gens[2].decay_function->register_emitter(adsr_exponential_decrement);
		env_gens[2].release_function->register_emitter(adsr_exponential_decrement);

		env_gens[2].attack_function->register_emitter(adsr_logaritmic_increment);
		env_gens[2].decay_function->register_emitter(adsr_exponential_decrement);
		env_gens[2].release_function->register_emitter(adsr_linear_decrement);

		hw.connect_jack(JACK_VOLUME, env_gens[0].value);
		hw.connect_jack(JACK_MOD, env_gens[1].value);
		hw.connect_jack(JACK_CUTOFF, env_gens[2].value);
		hw.connect_jack(JACK_PITCH, env_gens[3].value);

		gate_conv.input->register_emitter(gate_mix.output);
		hw.connect_jack(JACK_TRIGGER, gate_conv.output);

	}

	void unload_program()
	{}
private:
	adsr env_gens[4];
	mixer<bool> gate_mix;
	converter<bool, float> gate_conv;
};

#endif //ADSR_TEST_H
