#ifndef SHIFTER_H
#define SHIFTER_H

#include <base/component.h>

template <typename Value>
class shifter : public component {
public:
	shifter(EmitterP<Value> input = nullptr,
			EmitterP<Value> factor = nullptr):
		input(Receiver<Value>::make(input)),
		factor(Receiver<Value>::make(factor)),
		output(Emitter<Value>::make())
	{
		output->setUpdateFunction([&](State *state)->Value {
			return (this->input->getValue(state) +
				this->factor->getValue(state));
		});
	}

	ReceiverP<Value> input;
	ReceiverP<Value> factor;

	EmitterP<Value> output;
};

#endif //SHIFTER_H
