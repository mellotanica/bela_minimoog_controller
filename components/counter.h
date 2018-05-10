#ifndef COUNTER_H
#define COUNTER_H

#include <base/component.h>

class counter : public component {
public:
	counter():
		trigger(Receiver<bool>::make(False)),
		reset(Receiver<bool>::make(False)),
		value(Emitter<unsigned>::make()),
		val(0)
	{
		value->setUpdateFunction([&](State * state)->unsigned {
			val += trigger->getValue(state);
			val -= (val * reset->getValue(state));
			return val;
		});
	}

	ReceiverP<bool> trigger;
	ReceiverP<bool> reset;

	EmitterP<unsigned> value;
protected:
	unsigned val;
};

#endif //COUNTER_H
