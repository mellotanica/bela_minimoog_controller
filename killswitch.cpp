/***** killswitch.cpp *****/

#include <killswitch.h>

killswitch::killswitch(short pin, bool defaultState, unsigned int debounceMsecs):
	pin(pin),
	defaultState(defaultState),
	state(false),
	debounceMsecs(debounceMsecs),
	debounceTime(0),
	debouncing(false)
{}
	
void killswitch::setup(BelaContext *context, void *userData)
{
	pinMode(context, 0, pin, INPUT);
	debounceTime = (int) (debounceMsecs * context->digitalSampleRate / 1000.0);
}

void killswitch::read(BelaContext *context, void *userData, 
				unsigned int audioFrameCount, 
				unsigned int analogFrameCount, 
				unsigned int digitalFrameCount)
{
	bool tv = (digitalRead(context, 0, pin) == 0 ? false : true);
	
	if (debounceMsecs > 0) {
		if (!debouncing && tv != state) {
				debouncing = true;
				debounceCounter = debounceTime;
		} else {
			debounceCounter --;
			if (debounceCounter <= 0) {
				debouncing = false;
				if(tv != state) {
					state = tv;
					updateValue(context, state != defaultState, audioFrameCount, analogFrameCount, digitalFrameCount);
				}
			}
		}
	} else {
		if (state != tv) {
			state = tv;
			updateValue(context, state != defaultState, audioFrameCount, analogFrameCount, digitalFrameCount);
		}
	}
}