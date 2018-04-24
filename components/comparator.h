#ifndef COMPARATOR_H
#define COMPARATOR_H

#include <base/component.h>

#include <components/constant.h>

enum comparator_type
{
	LOW_PASS,
	HIGH_PASS,
	BAND_PASS,
	NOTCH
};

static auto low_pass_comparator = constant<comparator_type>::make(LOW_PASS);
static auto high_pass_comparator = constant<comparator_type>::make(HIGH_PASS);
static auto band_pass_comparator = constant<comparator_type>::make(BAND_PASS);
static auto notch_comparator = constant<comparator_type>::make(NOTCH);

template <typename Input>
class comparator: public component {
public:
	comparator(std::shared_ptr<Emitter<comparator_type>> comp_type = low_pass_comparator):
		type(Receiver<comparator_type>::make(comp_type)),
		threshold_a(Receiver<Input>::make()),
		threshold_b(Receiver<Input>::make()),
		input(Receiver<Input>::make()),
		output(Emitter<bool>::make())
	{
		output->setUpdateFunction([&](State *state)->bool {
			return this->evaluate(state);
		});
	}

	std::shared_ptr<Receiver<comparator_type>> type;
	std::shared_ptr<Receiver<Input>> threshold_a; //main threshold (inclusive)
	std::shared_ptr<Receiver<Input>> threshold_b; //(exclusive)

	std::shared_ptr<Receiver<Input>> input;
	std::shared_ptr<Emitter<bool>> output;

protected:
	bool evaluate(State *state){
		switch(type->getValue(state)) {
			case LOW_PASS:
				return input->getValue(state) <= threshold_a->getValue(state);
			case HIGH_PASS:
				return input->getValue(state) >= threshold_a->getValue(state);
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