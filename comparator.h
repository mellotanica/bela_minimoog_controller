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

static auto low_pass_comparator = std::make_shared<constant<comparator_type>>(LOW_PASS);
static auto high_pass_comparator = std::make_shared<constant<comparator_type>>(HIGH_PASS);
static auto band_pass_comparator = std::make_shared<constant<comparator_type>>(BAND_PASS);
static auto notch_comparator = std::make_shared<constant<comparator_type>>(NOTCH);

template <typename Input>
class comparator: public component {
public:
	comparator(comparator_type comp_type = LOW_PASS):
		type(std::make_shared<Receiver<comparator_type>>(LOW_PASS)),
		threshold_a(std::make_shared<Receiver<Input>>()),
		threshold_b(std::make_shared<Receiver<Input>>()),
		input(std::make_shared<Receiver<Input>>()),
		output(std::make_shared<Emitter<bool>>())
	{
		set_type(comp_type);
		output->setUpdateFunction([&](State *state)->bool {
			return this->evaluate(state);
		});
	}

	void set_type(comparator_type comp_type) 
	{
		switch(comp_type){
			case LOW_PASS:
				type->register_emitter(low_pass_comparator);
				break;
			case HIGH_PASS:
				type->register_emitter(high_pass_comparator);
				break;
			case BAND_PASS:
				type->register_emitter(band_pass_comparator);
				break;
			case NOTCH:
				type->register_emitter(notch_comparator);
				break;
		}
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
