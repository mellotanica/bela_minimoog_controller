#include <components/killswitch.h>

killswitch::killswitch(short pin, bool defaultState, unsigned int debounceMsecs):
	gate(Emitter<bool>::make()),
	trigger(Emitter<bool>::make()),
	pin(pin),
	defaultState((defaultState ? 1 : 0)),
	debounceMsecs(debounceMsecs),
	debounceTime(0),
	debouncing(false),
	prevState(false),
	triggerRequest(false)
{
	gate->setUpdateFunction([&](State *execState)->bool {
		if(this->getSwitchState(execState)) {
			this->triggerRequest = !this->gate->getLastValue();
			return true;
		}
		return false;
	});

	trigger->setUpdateFunction([&](State *execState)->bool {
		if(this->triggerRequest) {
			this->triggerRequest = false;
			return true;
		}
		return false;
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
		if (!debouncing && tv != gate->getLastValue()) {
				debouncing = true;
				debounceCounter = debounceTime;
		} else {
			debounceCounter --;
			if (debounceCounter <= 0) {
				debouncing = false;
				if(tv != gate->getLastValue()) {
					return tv;
				}
			}
		}
	} else {
		if (gate->getLastValue() != tv) {
			return tv;
		}
	}
	return gate->getLastValue();
}

