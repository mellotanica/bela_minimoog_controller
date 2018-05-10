#ifndef PULSE_MOD_H
#define PULSE_MOD_H

#include <base/program.h>

#include <hardware.h>

#include <memory>

#include <components/constant.h>
#include <components/lfo.h>
#include <components/mixer.h>
#include <components/smoother.h>
#include <components/binary_function.h>
#include <components/comparator.h>
#include <components/counter.h>
#include <components/adsr.h>
#include <components/converter.h>

#define MAX_STEPS 4

class pulse_mod : public program {
public:
	pulse_mod()
	{
		aftertouchFilter = std::make_shared<binary_function<float>>([](float aftertouch, float pot)->float{
			bool active = pot > 0.5;
			return (! active) + (aftertouch * active);
		});
		
		triggerFun = std::make_shared<binary_function<bool>>([](bool midi, bool ksw)->bool{
			return midi ^ ksw;
		});

	}

	/* POTs:    0 		1            2
	 *	lfo shape | skip step  |  lfo freq
	 *
	 * 	    3	 	4	     5
	 *        attack  | duty cycle | aftertouch 
	 */

	void load_program()
	{
		hardware& hw = hardware::getInstance();

		// LFO shape
		hw.pots[0]->maxv->register_emitter(lfo_shape_maxF);
		hw.pots[0]->error->register_emitter(integer_pot_error);

		convLfo.input->register_emitter(hw.pots[0]->value);
		osc.shape->register_emitter(convLfo.output);

		// LFO frequency
		hw.pots[2]->minv->register_emitter(constant<float>::make(0.1));
		hw.pots[2]->maxv->register_emitter(constant<float>::make(10));

		aftertouchFilter->inputA->register_emitter(hw.midi->aftertouchF.value);
		aftertouchFilter->inputA->register_emitter(hw.pots[5]->value);

		frequencyMix.mix_mode->register_emitter(mixer_mul);
		frequencyMix.register_input(hw.pots[2]->value);
		frequencyMix.register_input(aftertouchFilter->output);

		osc.frequency->register_emitter(frequencyMix.output);

		// LFO duty cycle
		osc.duty_cycle->register_emitter(hw.pots[4]->value);

		// LFO trigger
		triggerMix.mix_mode->register_emitter(mixer_or);
		triggerMix.register_input(hw.midi->trigger.value);
		triggerMix.register_input(hw.killswitches[0]->trigger);

		osc.reset_phase->register_emitter(triggerMix.output);

		// LFO cycles count
		counter_steps.trigger->register_emitter(osc.trigger);
		counter_steps.reset->register_emitter(triggerMix.output);

		// LFO skip
		hw.pots[1]->minv->register_emitter(OneF);
		hw.pots[1]->maxv->register_emitter(constant<float>::make(MAX_STEPS));
		
		converter_step.input->register_emitter(hw.pots[1]->value);

		comparator_skipstep.type->register_emitter(equals_comparator);
		comparator_skipstep.input->register_emitter(counter_steps.value);
		comparator_skipstep.threshold_a->register_emitter(converter_step.output);

		comparator_adsrstep.type->register_emitter(high_pass_exclusive_comparator);
		comparator_adsrstep.input->register_emitter(counter_steps.value);
		comparator_adsrstep.threshold_a->register_emitter(constant<unsigned>::make(MAX_STEPS));

		lfoMaster.mix_mode->register_emitter(mixer_and);
		lfoMaster.register_input(osc.value);
		
		converter_rev_skipstep.input->register_emitter(comparator_skipstep.outputNeg);
		lfoMaster.register_input(converter_rev_skipstep.output);
		
		converter_rev_adsrstep.input->register_emitter(comparator_adsrstep.outputNeg);
		lfoMaster.register_input(converter_rev_adsrstep.output);

		converter_rev_gate.input->register_emitter(triggerFun->output);
		lfoMaster.register_input(converter_rev_gate.output);

		// ADSR config
		triggerFun->inputA->register_emitter(hw.killswitches[0]->gate);
		triggerFun->inputB->register_emitter(hw.midi->gate.value);

		adsrTriggerMix.mix_mode->register_emitter(mixer_and);
		adsrTriggerMix.register_input(comparator_adsrstep.output);
		adsrTriggerMix.register_input(triggerFun->output);

		envelope.gate->register_emitter(adsrTriggerMix.output);

		hw.pots[3]->minv->register_emitter(adsr_default_mintime);
		hw.pots[3]->maxv->register_emitter(adsr_default_maxtime);
		envelope.attack->register_emitter(hw.pots[3]->value);
		
		envelope.attack_level->register_emitter(OneF);
		envelope.decay->register_emitter(ZeroF);
		envelope.sustain->register_emitter(OneF);
		envelope.release->register_emitter(adsr_default_time);

		// output connection
		outputMaster.mix_mode->register_emitter(mixer_add);
		outputMaster.register_input(lfoMaster.output);
		outputMaster.register_input(envelope.value);
		
		hw.connect_jack(JACK_CUTOFF, outputMaster.output);
		// hw.outJacks[5]->value->register_emitter(output_smoother.output);
	}

	void unload_program()
	{
		frequencyMix.clear_inputs();
		triggerMix.clear_inputs();
		lfoMaster.clear_inputs();
		outputMaster.clear_inputs();
	}

private:
	lfo osc;
	converter<float, unsigned> converter_step;
	converter<bool, float> converter_rev_skipstep;
	converter<bool, float> converter_rev_adsrstep;
	converter<bool, float> converter_rev_gate;
	comparator<unsigned> comparator_skipstep;
	comparator<unsigned> comparator_adsrstep;
	converter<float, lfo_shape> convLfo;
	mixer<float> frequencyMix;
	mixer<bool> triggerMix;
	mixer<bool> adsrTriggerMix;
	mixer<float> lfoMaster;
	mixer<float> outputMaster;
	std::shared_ptr<binary_function<float>> aftertouchFilter;
	std::shared_ptr<binary_function<bool>> triggerFun;
	counter counter_steps;
	adsr envelope;
};


#endif //PULSE_MOD_H
