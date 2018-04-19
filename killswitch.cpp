/***** killswitch.cpp *****/

#include <killswitch.h>

killswitch::killswitch(short pin, bool defaultState, unsigned int debounceMsecs):
	pin(pin),
	defaultState(defaultState),
	debounceMsecs(debounceMsecs),
	debounceTime(0),
	debouncing(false)
{
	state.setUpdateFunction([&](State *execState)->bool {
		return this->getSwitchState(execState);
	});
}
	
void killswitch::setup(BelaContext *context, void *userData)
{
	pinMode(context, 0, pin, INPUT);
	debounceTime = (int) (debounceMsecs * context->digitalSampleRate / 1000.0);
}

bool killswitch::getSwitchState(State *execState) 
{
	bool tv = (digitalRead(execState->context, 0, pin) == 0 ? false : true);
	
	if (debounceMsecs > 0) {
		if (!debouncing && tv != state.getLastValue()) {
				debouncing = true;
				debounceCounter = debounceTime;
		} else {
			debounceCounter --;
			if (debounceCounter <= 0) {
				debouncing = false;
				if(tv != state.getLastValue()) {
					return tv;
				}
			}
		}
	} else {
		if (state.getLastValue() != tv) {
			return tv;
		}
	}
	return state.getLastValue();
}

