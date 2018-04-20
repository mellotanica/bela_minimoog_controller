#ifndef FILTER_H
#define FILTER_H

#include <comparator.h>

template <typename Input>
class filter: public comparator {
	filter(Input default_value, comparator_type filter_type):
		comparator(filter_type),
		output(Emitter<Input>::make())
	{
		output->setUpdateFunction([&this,=default_value](State *state)->Input {
			if (this->evaluate(state)) {
				return this->input->getValue(state);
			} else {
				return default_value;
			}
		});
	}

	std::shared_ptr<Emitter<Input>> output;
};

#endif //FILTER_H
