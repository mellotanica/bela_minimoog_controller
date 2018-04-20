#ifndef PRINTER_H
#define PRINTER_H

#include <component.h>

template <typename Input>
class printer: public output {
public:
	printer(std::string fmt):
		input(std::make_shared<Receiver<Input>>()),
		output(std::make_shared<Emitter<Input>>()),
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

	std::shared_ptr<Receiver<Input>> input;
	std::shared_ptr<Emitter<Input>> output;
protected:
	Input prev_val;
	std::string fmt;
};

#endif //PRINTER_H
