#ifndef COMPARATOR_H
#define COMPARATOR_H

#include <component.h>
#include <constant.h>

enum comparator_type
{
	LOW_PASS,
	HIGH_PASS,
	BAND_PASS,
	NOTCH
};

static constant<comparator_type> low_pass_comparator(LOW_PASS);
static constant<comparator_type> high_pass_comparator(HIGH_PASS);
static constant<comparator_type> band_pass_comparator(BAND_PASS);
static constant<comparator_type> notch_comparator(NOTCH);

template <typename Input>
class comparator: public component {
public:
	comparator(comparator_type comp_type = LOW_PASS):
		type(LOW_PASS)
	{
		set_type(comp_type);
		output.setUpdateFunction([&](State *state)->bool {
			return this->evaluate(state);
		});
	}

	void set_type(comparator_type comp_type) 
	{
		switch(comp_type){
			case LOW_PASS:
				type.register_emitter(&low_pass_comparator);
				break;
			case HIGH_PASS:
				type.register_emitter(&high_pass_comparator);
				break;
			case BAND_PASS:
				type.register_emitter(&band_pass_comparator);
				break;
			case NOTCH:
				type.register_emitter(&notch_comparator);
				break;
		}
	}

	Receiver<comparator_type> type;
	Receiver<Input> threshold_a; //main threshold (inclusive)
	Receiver<Input> threshold_b; //(exclusive)

	Receiver<Input> input;
	Emitter<bool> output;

protected:
	bool evaluate(State *state){
		switch(type.getValue(state)) {
			case LOW_PASS:
				return input.getValue(state) <= threshold_a.getValue(state);
			case HIGH_PASS:
				return input.getValue(state) >= threshold_a.getValue(state);
			case BAND_PASS:
				return input.getValue(state) >= threshold_a.getValue(state) &&
					input.getValue(state) < threshold_b.getValue(state);
			case NOTCH:
				return input.getValue(state) <= threshold_a.getValue(state) ||
					input.getValue(state) > threshold_b.getValue(state);
		}
	}
};

#endif //COMPARATOR_H
