#ifndef MIDI_DEV_H
#define MIDI_DEV_H

#include <base/component.h>

#include <Midi.h>
#include <vector>

#include <components/variable.h>
#include <components/constant.h>

class midiDev : public output {
public: 
	midiDev(const char *port, bool debug_enabled = false);

	void setup(BelaContext *context, void *userData);
	void render(State *state);
	void reset();

	variableVector<int8_t> notes;
	
	variable<bool> trigger;
	variable<bool> gate;

	variable<int8_t> velocity;
	variable<float> velocityF;
	
	variable<int8_t> aftertouch;
	variable<float> aftertouchF;

	std::shared_ptr<Receiver<bool>> triggerSend;
private:
	const char *port;
	bool debug_enabled;

	int8_t last_note;
	
	bool triggering;
	int8_t triggering_note;
	int8_t triggering_velocity;

	Midi midi;

	bool trigger_request;

	friend void midiMessageCallback(MidiChannelMessage message, void *arg);
};

#endif //MIDI_DEV_H
