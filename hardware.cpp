#include <hardware.h>

#include <components/lfo.h>
#include <components/constant.h>
#include <components/comparator.h>
#include <components/converter.h>

hardware& hardware::getInstance()
{
	static hardware instance;
	return instance;
}

void hardware::connect_jack(unsigned int jack, std::shared_ptr<Emitter<float>> em)
{
	outJacks[jack]->value->register_emitter(em);
	
	leds[jack]->pwm_duty_cycle->register_emitter(em);
	leds[jack]->state->register_emitter(True);
	leds[jack]->pwm_period->register_emitter(led_pwm_period);
}

hardware::hardware():
	bypass(false)
{
	hw_components.insert(hw_components.end(), leds.begin(), leds.end());
	hw_components.insert(hw_components.end(), killswitches.begin(), killswitches.end());
	hw_components.insert(hw_components.end(), pots.begin(), pots.end());
	hw_components.insert(hw_components.end(), switches.begin(), switches.end());
	hw_components.insert(hw_components.end(), inJacks.begin(), inJacks.end());
	hw_components.insert(hw_components.end(), outJacks.begin(), outJacks.end());
	hw_components.push_back(midi);

	hw_outputs.insert(hw_outputs.end(), outJacks.begin(), outJacks.end());
	hw_outputs.insert(hw_outputs.end(), leds.begin(), leds.end());
	hw_outputs.push_back(midi);
} 

