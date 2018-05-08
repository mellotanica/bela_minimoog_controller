#ifndef FORWARDER_H
#define FORWARDER_H

#include <base/component.h>

template <typename Value>
class forwarder : public component {
public:
	forwarder(EmitterP <Value> input = nullptr):
		input(Receiver<Value>::make(input)),
		output(Emitter<Value>::make())
	{
		output.setUpdateFunction([&](State *state)->Value {
			return this->input->getValue(state);
		});
	};
	
	ReceiverP <Value> input;
	EmitterP <Value> output;
};

#endif //FORWARDER_H
