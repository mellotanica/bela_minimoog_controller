#include <coordinator.h>

#include <components/lfo.h>
#include <components/constant.h>
#include <components/comparator.h>
#include <components/converter.h>

coordinator& coordinator::getInstance()
{
	static coordinator instance;
	return instance;
}

coordinator::coordinator()
{
	hw_components.insert(hw_components.end(), leds.begin(), leds.end());
	hw_components.insert(hw_components.end(), killswitches.begin(), killswitches.end());
	hw_components.insert(hw_components.end(), pots.begin(), pots.end());
	hw_components.insert(hw_components.end(), switches.begin(), switches.end());
	hw_components.insert(hw_components.end(), inJacks.begin(), inJacks.end());
	hw_components.insert(hw_components.end(), outJacks.begin(), outJacks.end());
	hw_components.push_back(midi);

	active_outputs.insert(active_outputs.end(), leds.begin(), leds.end());
	active_outputs.insert(active_outputs.end(), outJacks.begin(), outJacks.end());


	/*
	 * test
	 */

	pots[0]->minv->register_emitter(OneF);
	pots[0]->maxv->register_emitter(constant<float>::make(4));
	pots[0]->error->register_emitter(integer_pot_error);

	pots[2]->minv->register_emitter(constant<float>::make(0.5));
	pots[2]->maxv->register_emitter(led_pwm_period);

	for(int i = 1; i < 4; i++){
		auto comp = new comparator<float>(band_pass_comparator);

		comp->threshold_a->register_emitter(constant<float>::make(i));
		comp->threshold_b->register_emitter(constant<float>::make(i+1));
		comp->input->register_emitter(pots[0]->value);

		leds[i]->state->register_emitter(comp->output);
		leds[i]->pwm_duty_cycle->register_emitter(pots[1]->value);
		leds[i]->pwm_period->register_emitter(pots[2]->value);
	}

	auto osc = new lfo(square_lfo);

	pots[4]->minv->register_emitter(constant<float>::make(0.1));
	pots[4]->maxv->register_emitter(constant<float>::make(10));

	pots[3]->maxv->register_emitter(constant<float>::make(INV_RAMP+1));
	pots[3]->error->register_emitter(integer_pot_error);

	auto conv = new converter<float,lfo_shape>(pots[3]->value);

	osc->shape->register_emitter(conv->output);

	osc->frequency->register_emitter(pots[4]->value);

	osc->duty_cycle->register_emitter(pots[5]->value);

	osc->reset_phase->register_emitter(killswitches[0]->state);

	leds[4]->state->register_emitter(True);
	leds[4]->pwm_duty_cycle->register_emitter(osc->value);

	leds[0]->pwm_period->register_emitter(OneF);
	leds[0]->pwm_duty_cycle->register_emitter(OneF);
	leds[0]->state->register_emitter(killswitches[0]->state);
} 

