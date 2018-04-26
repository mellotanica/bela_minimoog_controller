#include <components/mididev.h>

void midiMessageCallback(MidiChannelMessage message, void *arg)
{
	midiDev *midi = (midiDev *) arg;
	if (midi->debug_enabled) {
		message.prettyPrint();
	}
	switch (message.getType()) {
		case kmmNoteOn:
			// handle trigger
			midi->trigger_request = true;
			
			// store last played note for trigger
			midi->last_note = message.getDataByte(0);

			// handle note
			midi->notes.push_back(midi->last_note);

			// handle velocity
			midi->velocity.set(message.getDataByte(1));
			midi->velocityF.set(((float)midi->velocity.get()) / 127);
			
			break;
		case kmmNoteOff:
			{
				// handle note
				std::vector<int8_t> tv = midi->notes.get();
				int8_t note = message.getDataByte(0);
				for(auto n = tv.begin(); n != tv.end(); ++n) {
					if((*n) == note) {
						tv.erase(n);
						break;
					}
				}
				midi->notes.set(tv);
			}

			// reset values if no note is on
			if (midi->notes.getSize() <= 0) {
				midi->velocity.set(-1);
				midi->velocityF.set(-1);
				midi->aftertouch.set(-1);
				midi->aftertouchF.set(-1);
			}
			
			break;
		case kmmChannelPressure:
			midi->aftertouch.set(message.getDataByte(0));
			midi->aftertouchF.set(((float)midi->aftertouch.get()) / 127);
			break;
		default:
			break;
	}
}

midiDev::midiDev(const char *port, bool debug_enabled):
	triggerSend(Receiver<bool>::make(False)),
	port(port),
	debug_enabled(debug_enabled),
	triggering(false)
{
	trigger.value->setUpdateFunction([&](State *state)->bool {
		if(this->trigger_request) {
			this->trigger_request = false;
			return true;
		}
		return false;
	});

	gate.value->setUpdateFunction([&](State *state)->bool {
		return (this->notes.getSize() > 0);
	});
}

void midiDev::setup(BelaContext *context, void *userData)
{
	midi.readFrom(port);
	midi.enableParser(true);
	midi.setParserCallback(midiMessageCallback, (void *) this);
}

void midiDev::render(State *state)
{
	if(triggering){
		midi.writeNoteOn(0, triggering_note, triggering_velocity);
		triggering = false;
		if(debug_enabled){
			rt_printf("writing Note ON, channel 0, note %d, velocity %d\n",
					triggering_note, triggering_velocity);
		}
	} else if(triggerSend->getValue(state)) {
		triggering_note = last_note;
		triggering_velocity = velocity.get();
		midi.writeNoteOff(0, triggering_note, 0);
		triggering = true;
		if(debug_enabled){
			rt_printf("writing Note OFF, channel 0, note %d, velocity 0\n",
					triggering_note);
		}
	}
}

void midiDev::reset()
{
	triggerSend->register_emitter(False);
}

