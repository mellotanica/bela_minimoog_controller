#ifndef TREMOLO_SYNC_H
#define TREMOLO_SYNC_H

#include <base/program.h>

#include <hardware.h>

#include <memory>

#include <components/constant.h>
#include <components/lfo.h>
#include <components/mixer.h>
#include <components/smoother.h>
#include <components/binary_function.h>

class tremolo_sync : public program {
public:
	tremolo_sync()
	{
		aftertouchFilter = std::make_shared<binary_function<float>>([](float aftertouch, float pot)->float{
			bool active = pot > 0.5;
			return (! active) + (aftertouch * active);
		});
	}

	void load_program()
	{
		hardware& hw = hardware::getInstance();

		// LFO shape
		hw.pots[0]->maxv->register_emitter(lfo_shape_maxF);
		hw.pots[0]->error->register_emitter(integer_pot_error);

		convLfo.input->register_emitter(hw.pots[0]->value);
		osc.shape->register_emitter(convLfo.output);

		// LFO frequency
		hw.pots[1]->minv->register_emitter(constant<float>::make(0.1));
		hw.pots[1]->maxv->register_emitter(constant<float>::make(50));

		aftertouchFilter->inputA->register_emitter(hw.midi->aftertouchF.value);
		aftertouchFilter->inputA->register_emitter(hw.pots[3]->value);

		frequencyMix.mix_mode->register_emitter(mixer_mul);
		frequencyMix.register_input(hw.pots[1]->value);
		frequencyMix.register_input(aftertouchFilter->output);

		osc.frequency->register_emitter(frequencyMix.output);

		// LFO duty cycle
		osc.duty_cycle->register_emitter(hw.pots[2]->value);

		// LFO trigger
		triggerMix.mix_mode->register_emitter(mixer_or);
		triggerMix.register_input(hw.midi->trigger.value);
		triggerMix.register_input(hw.killswitches[0]->trigger);

		osc.reset_phase->register_emitter(triggerMix.output);

		// output volume
		outputMaster.mix_mode->register_emitter(mixer_mul);
		outputMaster.register_input(osc.value);
		outputMaster.register_input(hw.pots[4]->value);

		// smoother
		output_smoother.input->register_emitter(outputMaster.output);

		// output connection
		hw.connect_jack(JACK_VOLUME, output_smoother.output);
		// hw.outJacks[5]->value->register_emitter(output_smoother.output);
	}

	void unload_program()
	{
		frequencyMix.clear_inputs();
		triggerMix.clear_inputs();
		outputMaster.clear_inputs();
	}

private:
	lfo osc;
	converter<float, lfo_shape> convLfo;
	mixer<float> frequencyMix;
	mixer<bool> triggerMix;
	mixer<float> outputMaster;
	smoother output_smoother;
	std::shared_ptr<binary_function<float>> aftertouchFilter;
};


#endif //TREMOLO_SYNC_H
