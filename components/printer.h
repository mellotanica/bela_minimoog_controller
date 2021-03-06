#ifndef PRINTER_H
#define PRINTER_H

#include <base/component.h>

template <typename Input>
class printer: public output {
public:
	printer(std::string fmt,
			EmitterP<Input> input = nullptr):
		input(Receiver<Input>::make(input)),
		output(Emitter<Input>::make()),
		fmt(fmt)
	{
		output->setUpdateFunction([&](State *state)->Input {
			return this->print(state);
		});
	}

	void render(State *state)
	{
		print(state);
	}

	Input print(State *state)
	{
		if(input->getValue(state) != prev_val){
			prev_val = input->getValue(state);
			rt_printf(fmt.c_str(), prev_val);
		}
		return prev_val;
	}

	ReceiverP<Input> input;
	EmitterP<Input> output;
protected:
	Input prev_val;
	std::string fmt;
};

#endif //PRINTER_H
