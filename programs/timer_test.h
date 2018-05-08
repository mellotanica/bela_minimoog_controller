#ifndef TIMER_TEST_H
#define TIMER_TEST_H

#include <base/program.h>

#include <components/timer.h>
#include <components/constant.h>
#include <components/comparator.h>
#include <components/function_runner.h>

class timer_test : public program {
public:
	timer_test()
	{
		led_state = false;
		triggerFun = std::make_shared<function_runner<bool>>([&](bool triggered)->bool {
			if(triggered) {
				this->led_state = !this->led_state;
				hardware::getInstance().leds[0]->state->register_emitter(
						this->led_state ? True : False);
			}
			return triggered;
		});
	}

	void load_program()
	{
		hardware& hw = hardware::getInstance();

		hw.pots[0]->minv->register_emitter(constant<float>::make(0.2));
		hw.pots[0]->maxv->register_emitter(constant<float>::make(5));

		main_t.duration->register_emitter(hw.pots[0]->value);
		
		oneShot_comp.input->register_emitter(hw.pots[1]->value);
		oneShot_comp.threshold_a->register_emitter(constant<float>::make(0.5));

		main_t.oneShot->register_emitter(oneShot_comp.output);
		
		main_t.reset->register_emitter(hw.killswitches[0]->trigger);

		triggerFun->input->register_emitter(main_t.trigger);

		hw.active_outputs.push_back(&(*triggerFun));

		hw.leds[2]->state->register_emitter(main_t.running);
	}

	void unload_program()
	{}
private:
	bool led_state;

	timer main_t;
	comparator<float> oneShot_comp;
	std::shared_ptr<function_runner<bool>> triggerFun;
};

#endif //TIMER_TEST_H
