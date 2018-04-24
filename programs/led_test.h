#ifndef LED_TEST_H
#define LED_TEST_H

#include <base/program.h>

#include <coordinator.h>

#include <functional>

#include <components/constant.h>
#include <components/comparator.h>
#include <components/lfo.h>
#include <components/converter.h>

class led_test : public program {
public:
	void load_program() 
	{
		coordinator& coord = coordinator::getInstance();
		coord.pots[0]->minv->register_emitter(OneF);
		coord.pots[0]->maxv->register_emitter(constant<float>::make(4));
		coord.pots[0]->error->register_emitter(integer_pot_error);

		coord.pots[2]->minv->register_emitter(constant<float>::make(0.5));
		coord.pots[2]->maxv->register_emitter(led_pwm_period);
		coord.pots[2]->error->register_emitter(default_pot_error);

		for(int i = 1; i < 4; i++){
			comp[i-1].type->register_emitter(band_pass_comparator);
			comp[i-1].threshold_a->register_emitter(constant<float>::make(i));
			comp[i-1].threshold_b->register_emitter(constant<float>::make(i+1));
			comp[i-1].input->register_emitter(coord.pots[0]->value);

			coord.leds[i]->state->register_emitter(comp[i-1].output);
			coord.leds[i]->pwm_duty_cycle->register_emitter(coord.pots[1]->value);
			coord.leds[i]->pwm_period->register_emitter(coord.pots[2]->value);
		}

		coord.pots[4]->minv->register_emitter(constant<float>::make(0.1));
		coord.pots[4]->maxv->register_emitter(constant<float>::make(10));
		coord.pots[4]->error->register_emitter(default_pot_error);

		coord.pots[3]->minv->register_emitter(ZeroF);
		coord.pots[3]->maxv->register_emitter(constant<float>::make(INV_RAMP+1));
		coord.pots[3]->error->register_emitter(integer_pot_error);

		conv.input->register_emitter(coord.pots[3]->value);

		osc.shape->register_emitter(conv.output);

		osc.frequency->register_emitter(coord.pots[4]->value);

		osc.duty_cycle->register_emitter(coord.pots[5]->value);

		osc.reset_phase->register_emitter(coord.killswitches[0]->state);

		coord.leds[4]->state->register_emitter(True);
		coord.leds[4]->pwm_period->register_emitter(led_pwm_period);
		coord.leds[4]->pwm_duty_cycle->register_emitter(osc.value);

		coord.leds[0]->pwm_period->register_emitter(OneF);
		coord.leds[0]->pwm_duty_cycle->register_emitter(OneF);
		coord.leds[0]->state->register_emitter(coord.killswitches[0]->state);

		for(auto l : coord.leds){
			coord.active_outputs.push_back(l);
		}
	}

	void unload_program()
	{
	}
private:
	comparator<float> comp[3];
	lfo osc;
	converter<float, lfo_shape> conv;
};

#endif //LED_TEST_H
