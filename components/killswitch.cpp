#include <components/killswitch.h>

killswitch::killswitch(short pin, bool defaultState, unsigned int debounceMsecs):
	state(Emitter<bool>::make()),
	pin(pin),
	defaultState((defaultState ? 1 : 0)),
	debounceMsecs(debounceMsecs),
	debounceTime(0),
	debouncing(false)
{
	state->setUpdateFunction([&](State *execState)->bool {
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
	bool tv = digitalRead(execState->context, 0, pin) != defaultState;
	
	if (debounceMsecs > 0) {
		if (!debouncing && tv != state->getLastValue()) {
				debouncing = true;
				debounceCounter = debounceTime;
		} else {
			debounceCounter --;
			if (debounceCounter <= 0) {
				debouncing = false;
				if(tv != state->getLastValue()) {
					return tv;
				}
			}
		}
	} else {
		if (state->getLastValue() != tv) {
			return tv;
		}
	}
	return state->getLastValue();
}

