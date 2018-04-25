#ifndef HARDWARE_H
#define HARDWARE_H

#include <base/component.h>
#include <base/program.h>

#include <memory>
#include <vector>

#include <components/led.h>
#include <components/pot.h>
#include <components/killswitch.h>
#include <components/switch.h>
#include <components/jack.h>
#include <components/midiin.h>

#define JACK_VOLUME	0
#define JACK_TRIGGER	1
#define JACK_MOD	2
#define JACK_PITCH	3
#define JACK_CUTOFF	4

class hardware {
public:
	static hardware& getInstance();

	bool bypass;

	void connect_jack(unsigned int jack, std::shared_ptr<Emitter<float>> em);

	std::vector<component *> hw_components;
	std::vector<output *> hw_outputs;
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
		new outputJack(3, OneF), // Loudness
		new outputJack(0, ZeroF), // V-Trigger
		new outputJack(2, ZeroF), // Mod Source
		new outputJack(1, ZeroF), // Oscillator pitch
		new outputJack(4, ZeroF), // Filter Cutoff
	};

	midiIn * midi = new midiIn("hw:1,0,0");

private:
	hardware();

public:
	// delete copy constructors
	hardware(hardware const&) = delete;
	void operator=(hardware const&) = delete;
};

#endif //HARDWARE_H
