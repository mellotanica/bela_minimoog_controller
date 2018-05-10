#ifndef ADSR_STK_TEST_H
#define ADSR_STK_TEST_H

#include <base/program.h>

#include <components/mixer.h>
#include <components/binary_function.h>
#include <components/adsr_stk.h>
#include <components/converter.h>

class adsr_stk_test: public program {
public:
	adsr_stk_test()
	{
		triggerFun = std::make_shared<binary_function<bool>>([](bool midi, bool ksw)->bool{
			return midi ^ ksw;
		});
	}

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

		// triggerFun->inputA->register_emitter(gate_mix.output);
		// triggerFun->inputB->register_emitter(hw.midi->trigger.value);
		
		triggerFun->inputA->register_emitter(hw.killswitches[0]->gate);
		triggerFun->inputB->register_emitter(hw.midi->gate.value);

		env_gen.attack->register_emitter(hw.pots[0]->value);
		env_gen.decay->register_emitter(hw.pots[1]->value);
		env_gen.release->register_emitter(hw.pots[2]->value);

		env_gen.attack_level->register_emitter(hw.pots[3]->value);
		env_gen.sustain->register_emitter(hw.pots[4]->value);
		env_gen.gate->register_emitter(triggerFun->output);

		hw.connect_jack(JACK_VOLUME, env_gen.value);
		hw.connect_jack(JACK_MOD, env_gen.value);
		hw.connect_jack(JACK_CUTOFF, env_gen.value);
		hw.connect_jack(JACK_PITCH, env_gen.value);


		gate_conv.input->register_emitter(gate_mix.output);
		hw.connect_jack(JACK_TRIGGER, gate_conv.output);
	}

	void unload_program()
	{
		gate_mix.clear_inputs();
	}
private:
	mixer<bool> gate_mix;
	std::shared_ptr<binary_function<bool>> triggerFun;
	adsr_stk env_gen;
	converter<bool, float> gate_conv;
};

#endif //ADSR_TEST_H
