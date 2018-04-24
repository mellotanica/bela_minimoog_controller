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

void hardware::activate_program(std::shared_ptr<program> new_prog)
{
	bypass = true;
	if(active_prog) {
		active_prog->unload_program();
	}
	active_outputs.clear();
	new_prog->load_program();
	active_prog = new_prog;
	bypass = false;
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
} 

