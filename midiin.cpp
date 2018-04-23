#include <midiin.h>

void midiMessageCallback(MidiChannelMessage message, void *arg)
{
	midiIn *midiDev = (midiIn *) arg;
	if (midiDev->debug_enabled) {
		message.prettyPrint();
	}
	switch (message.getType()) {
		case kmmNoteOn:
			// handle trigger
			midiDev->trigger_request = true;
			
			// handle note
			midiDev->notes.push_back(message.getDataByte(0));

			// handle velocity
			midiDev->velocity.set(message.getDataByte(1));
			midiDev->velocityF.set(((float)midiDev->velocity.get()) / 127);
			
			break;
		case kmmNoteOff:
			{
				// handle note
				std::vector<int8_t> tv = midiDev->notes.get();
				int8_t note = message.getDataByte(0);
				for(auto n = tv.begin(); n != tv.end(); ++n) {
					if((*n) == note) {
						tv.erase(n);
						break;
					}
				}
				midiDev->notes.set(tv);
			}

			// reset values if no note is on
			if (midiDev->notes.getSize() <= 0) {
				midiDev->velocity.set(-1);
				midiDev->velocityF.set(-1);
				midiDev->aftertouch.set(-1);
				midiDev->aftertouchF.set(-1);
			}
			
			break;
		case kmmPolyphonicKeyPressure:
			midiDev->aftertouch.set(message.getDataByte(1));
			midiDev->aftertouchF.set(((float)midiDev->aftertouch.get()) / 127);
			break;
		case kmmChannelPressure:
			midiDev->aftertouch.set(message.getDataByte(0));
			midiDev->aftertouchF.set(((float)midiDev->aftertouch.get()) / 127);
			break;
		default:
			break;
	}
}

midiIn::midiIn(const char *port, bool debug_enabled):
	port(port),
	debug_enabled(debug_enabled)
{
	trigger.value->setUpdateFunction([&](State *state)->bool {
		if(this->trigger_request) {
			this->trigger_request = false;
			return true;
		}
		return false;
	});
}

void midiIn::setup(BelaContext *context, void *userData)
{
	midi.readFrom(port);
	midi.enableParser(true);
	midi.setParserCallback(midiMessageCallback, (void *) this);
}

