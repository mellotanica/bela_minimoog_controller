#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <base/component.h>
#include <vector>

#include <components/led.h>
#include <components/pot.h>
#include <components/killswitch.h>
#include <components/switch.h>
#include <components/jack.h>
#include <components/midiin.h>

class coordinator {
public:
	static coordinator& getInstance();

	std::vector<component *> hw_components;
	std::vector<output *> active_outputs;

	std::vector<led *> leds = {
		new led(P8_09),
		new led(P8_08),
		new led(P8_11),
		new led(P8_12),
		new led(P8_10),
	};

	std::vector<pot *> pots = {
		new pot(3),
		new pot(5),
		new pot(1),
		new pot(2),
		new pot(4),
		new pot(0),
	};

	std::vector<killswitch *> killswitches = {
		new killswitch(P8_07, true, 0),
	};

	std::vector<switch_comp *> switches = {
		new switch_comp(P8_15, P8_16, ON_OFF_ON),	
	};

	std::vector<inputJack *> inJacks = {
		new inputJack(7),
	};

	std::vector<outputJack *> outJacks = {
		new outputJack(0),
		new outputJack(1),
		new outputJack(2),
		new outputJack(3),
		new outputJack(4),
	};

	midiIn * midi = new midiIn("hw:1,0,0", true);

private:
	coordinator();

public:
	// delete copy constructors
	coordinator(coordinator const&) = delete;
	void operator=(coordinator const&) = delete;
};

#endif //COORDINATOR_H
