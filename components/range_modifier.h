#ifndef RANGE_MODIFIER_H
#define RANGE_MODIFIER_H

#include <base/component.h>
#include <utility>

template <typename Value>
class rangeModifier : public component {
public:
	rangeModifier(EmitterP<Value> input = nullptr,
			EmitterP<Value> minInput = nullptr,
			EmitterP<Value> maxInput = nullptr,
			EmitterP<Value> minOutput = nullptr,
			EmitterP<Value> maxOutput = nullptr):
		input(Receiver<Value>::make(input)),
		minInput(Receiver<Value>::make(minInput)),
		maxInput(Receiver<Value>::make(maxInput)),
		maxOutput(Receiver<Value>::make(maxOutput)),
		minOutput(Receiver<Value>::make(minOutput)),
		output(Emitter<Value>::make())
	{
		output->setUpdateFunction([&](State *state)->Value {
			return map(this->input->getValue(state),
					this->minInput->getValue(state),
					this->maxInput->getValue(state),
					this->minOutput->getValue(state),
					this->maxOutput->getValue(state));
		});
	}

	ReceiverP<Value> input;

	ReceiverP<Value> minInput;
	ReceiverP<Value> maxInput;

	ReceiverP<Value> minOutput;
	ReceiverP<Value> maxOutput;

	EmitterP<Value> output;
};

#endif //RANGE_MODIFIER_H
