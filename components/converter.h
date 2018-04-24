#ifndef CONVERTER_H
#define CONVERTER_H

#include <base/component.h>

template <typename from, typename to>
class converter: public component {
public:
	converter(std::shared_ptr<Emitter<from>> source = nullptr):
		input(Receiver<from>::make(source)),
		output(Emitter<to>::make())
	{
		output->setUpdateFunction([&](State *state)->to{
			return static_cast<to>(this->input->getValue(state));
		});	
	}
	
	std::shared_ptr<Receiver<from>> input;

	std::shared_ptr<Emitter<to>> output;
};

#endif //CONVERTER_H
