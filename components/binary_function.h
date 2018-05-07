#ifndef BINARY_FUNCTION_H
#define BINARY_FUNCTION_H

#include <components/function_runner.h>

template <typename Input>
class binary_function : public component {
public:
	binary_function(std::function<Input(Input, Input)> function):
		inputA(Receiver<Input>::make()),
		inputB(Receiver<Input>::make()),
		output(Emitter<Input>::make()),
		function(function)
	{
		register_output();
	}

	binary_function(std::function<Input(Input, Input)> function, 
			EmitterP<Input> emA, 
			EmitterP<Input> emB):
		inputA(Receiver<Input>::make(emA)),
		inputB(Receiver<Input>::make(emB)),
		output(Emitter<Input>::make()),
		function(function)
	{
		register_output();
	}

	virtual void render(State *state) { function(inputA->getValue(state), inputB->getValue(state)); }

	ReceiverP<Input> inputA;
	ReceiverP<Input> inputB;

	EmitterP<Input> output;
protected:
	std::function<Input(Input, Input)> function;

	void register_output()
	{
		output->setUpdateFunction([&](State *state)->Input {
			return function(inputA->getValue(state), inputB->getValue(state));
		});
	}
};

#endif //BINARY_FUNCTION_H
