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
			std::shared_ptr<Emitter<Input>> emA, 
			std::shared_ptr<Emitter<Input>> emB):
		inputA(Receiver<Input>::make(emA)),
		inputB(Receiver<Input>::make(emB)),
		output(Emitter<Input>::make()),
		function(function)
	{
		register_output();
	}

	virtual void render(State *state) { function(inputA->getValue(state), inputB->getValue(state)); }

	std::shared_ptr<Receiver<Input>> inputA;
	std::shared_ptr<Receiver<Input>> inputB;

	std::shared_ptr<Emitter<Input>> output;
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
