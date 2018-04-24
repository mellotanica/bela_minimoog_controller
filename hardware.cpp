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

