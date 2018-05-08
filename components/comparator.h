#ifndef COMPARATOR_H
#define COMPARATOR_H

#include <base/component.h>

#include <components/constant.h>

enum comparator_type
{
	LOW_PASS,
	LOW_PASS_EXCLUSIVE,
	HIGH_PASS,
	HIGH_PASS_EXCLUSIVE,
	BAND_PASS,
	NOTCH
};

static auto comparator_max_modeF = constant<float>::make(NOTCH+1);

static auto low_pass_comparator = constant<comparator_type>::make(LOW_PASS);
static auto low_pass_exclusive_comparator = constant<comparator_type>::make(LOW_PASS_EXCLUSIVE);
static auto high_pass_comparator = constant<comparator_type>::make(HIGH_PASS);
static auto high_pass_exclusive_comparator = constant<comparator_type>::make(HIGH_PASS_EXCLUSIVE);
static auto band_pass_comparator = constant<comparator_type>::make(BAND_PASS);
static auto notch_comparator = constant<comparator_type>::make(NOTCH);

template <typename Input>
class comparator: public component {
public:
	comparator(EmitterP<comparator_type> comp_type = low_pass_comparator,
			EmitterP<Input> input = nullptr,
			EmitterP<Input> threshold_a = nullptr,
			EmitterP<Input> threshold_b = nullptr):
		type(Receiver<comparator_type>::make(comp_type)),
		threshold_a(Receiver<Input>::make(threshold_a)),
		threshold_b(Receiver<Input>::make(threshold_b)),
		input(Receiver<Input>::make(input)),
		output(Emitter<bool>::make())
	{
		output->setUpdateFunction([&](State *state)->bool {
			return this->evaluate(state);
		});
	}

	ReceiverP<comparator_type> type;
	ReceiverP<Input> threshold_a; //main threshold (inclusive)
	ReceiverP<Input> threshold_b; //(exclusive)

	ReceiverP<Input> input;
	EmitterP<bool> output;

protected:
	inline bool evaluate(State *state){
		switch(type->getValue(state)) {
			case LOW_PASS:
				return input->getValue(state) <= threshold_a->getValue(state);
			case LOW_PASS_EXCLUSIVE:
				return input->getValue(state) < threshold_a->getValue(state);
			case HIGH_PASS:
				return input->getValue(state) >= threshold_a->getValue(state);
			case HIGH_PASS_EXCLUSIVE:
				return input->getValue(state) > threshold_a->getValue(state);
			case BAND_PASS:
				return input->getValue(state) >= threshold_a->getValue(state) &&
					input->getValue(state) < threshold_b->getValue(state);
			case NOTCH:
				return input->getValue(state) <= threshold_a->getValue(state) ||
					input->getValue(state) > threshold_b->getValue(state);
		}
	}
};

#endif //COMPARATOR_H
