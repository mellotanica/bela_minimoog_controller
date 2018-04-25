#ifndef SYNC_MOD_H
#define SYNC_MOD_H

#include <base/program.h>

#include <hardware.h>

#include <components/constant.h>
#include <components/lfo.h>
#include <components/mixer.h>

class sync_mod : public program {
public:
	void load_program()
	{
		hardware& hw = hardware::getInstance();
		
		hw.pots[0]->minv->register_emitter(ZeroF);
		hw.pots[0]->maxv->register_emitter(lfo_shape_maxF);
		hw.pots[0]->error->register_emitter(integer_pot_error);

		convLfo.input->register_emitter(hw.pots[0]->value);
		osc.shape->register_emitter(convLfo.output);

		hw.pots[3]->minv->register_emitter(ZeroF);
		hw.pots[3]->maxv->register_emitter(mixer_max_modeF);
		hw.pots[3]->error->register_emitter(integer_pot_error);

		convMix.input->register_emitter(hw.pots[3]->value);
		mix.mix_mode->register_emitter(convMix.output);

		hw.pots[1]->minv->register_emitter(constant<float>::make(0.1));
		hw.pots[1]->maxv->register_emitter(constant<float>::make(100));
		hw.pots[1]->error->register_emitter(default_pot_error);

		mix.register_input(hw.pots[1]->value);
		mix.register_input(hw.midi->aftertouchF.value);

		osc.frequency->register_emitter(mix.output);

		hw.pots[2]->minv->register_emitter(ZeroF);
		hw.pots[2]->maxv->register_emitter(OneF);
		hw.pots[2]->error->register_emitter(default_pot_error);
		
		osc.duty_cycle->register_emitter(hw.pots[2]->value);

		osc.reset_phase->register_emitter(hw.midi->trigger.value);

		hw.leds[JACK_MOD]->pwm_duty_cycle->register_emitter(osc.value);
		hw.leds[JACK_MOD]->pwm_period->register_emitter(led_pwm_period);
		hw.leds[JACK_MOD]->state->register_emitter(True);

		hw.
		
		hw.active_outputs.push_back(hw.leds[JACK_MOD]);
	}

	void unload_program()
	{
		mix.clear_inputs();
	}

private:
	lfo osc;
	converter<float, lfo_shape> convLfo;
	converter<float, mixer_type> convMix;
	mixer<float> mix;
};


#endif //SYNC_MOD_H
