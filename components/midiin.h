#ifndef MIDI_IN_H
#define MIDI_IN_H

#include <base/component.h>

#include <Midi.h>
#include <vector>

#include <components/variable.h>

class midiIn : public component {	
public: 
	midiIn(const char *port, bool debug_enabled = false);

	void setup(BelaContext *context, void *userData);

	variableVector<int8_t> notes;
	
	variable<bool> trigger;
	variable<bool> gate;

	variable<int8_t> velocity;
	variable<float> velocityF;
	
	variable<int8_t> aftertouch;
	variable<float> aftertouchF;
private:
	const char *port;
	bool debug_enabled;

	Midi midi;

	bool trigger_request;

	friend void midiMessageCallback(MidiChannelMessage message, void *arg);
};

#endif //MIDI_IN_H
