#ifndef AMPLIFIER_H
#define AMPLIFIER_H

#include <base/component.h>
#include <components/constant.h>

enum amplifier_type 
{
	LINEAR,
	EXPONENTIAL
};

static auto amplifier_linear = constant<amplifier_type>::make(LINEAR);
static auto amplifier_exponential = constant<amplifier_type>::make(EXPONENTIAL);

template <typename Value>
class amplifier : public component {
public:
	amplifier(EmitterP<Value> input = nullptr,
			EmitterP<Value> factor = nullptr,
			EmitterP<amplifier_type> type = amplifier_linear):
		input(Receiver<Value>::make(input)),
		factor(Receiver<Value>::make(factor)),
		type(Receiver<amplifier_type>::make(type)),
		output(Emitter<Value>::make())
	{
		output->setUpdateFunction([&](State *state)->Value {
			switch(this->type->getValue(state)) {
				case LINEAR:
					return (this->input->getValue(state) *
						this->factor->getValue(state));
				case EXPONENTIAL:
					return (this->input->getValue(state) **
						this->factor->getValue(state));
			}
		});
	}

	ReceiverP<Value> input;
	ReceiverP<Value> factor;

	ReceiverP<amplifier_type> type;

	EmitterP<Value> output;
};

#endif //AMPLIFIER_H
