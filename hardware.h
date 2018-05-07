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
#include <components/mididev.h>

#define JACK_VOLUME	0
#define JACK_MOD	1
#define JACK_PITCH	2
#define JACK_CUTOFF	3
#define JACK_TRIGGER	4

#define OUTPUT_LIMIT 0.9

class hardware {
public:
	static hardware& getInstance();

	bool bypass;

	void connect_jack(unsigned int jack, EmitterP<float> em);

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
		new outputJack(3, OUTPUT_LIMIT, OneF), // Loudness
		new outputJack(2, OUTPUT_LIMIT, ZeroF), // Mod Source
		new outputJack(1, 1, ZeroF), // Oscillator pitch
		new outputJack(4, 1, ZeroF), // Filter Cutoff
		new outputJack(0, 1, ZeroF), // V-Trigger
	};

	midiDev * midi = new midiDev("hw:1,0,0");

private:
	hardware();

public:
	// delete copy constructors
	hardware(hardware const&) = delete;
	void operator=(hardware const&) = delete;
};

#endif //HARDWARE_H
