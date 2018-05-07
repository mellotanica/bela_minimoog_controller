#ifndef FUNCTION_RUNNER_H
#define FUNCTION_RUNNER_H

#include <components/dummy.h>

#include <functional>

template <typename Input>
class function_runner : public dummy<Input> {
public:
	function_runner(std::function<Input(Input)> function):
		dummy<Input>(),
		output(Emitter<Input>::make()),
		function(function)
	{
		register_output();
	}

	function_runner(std::function<Input(Input)> function, EmitterP<Input> em):
		dummy<Input>(em),
		output(Emitter<Input>::make()),
		function(function)
	{
		register_output();
	}

	virtual void render(State *state) { function(dummy<Input>::input->getValue(state)); }

	EmitterP<Input> output;
protected:
	std::function<Input(Input)> function;

	void register_output()
	{
		output->setUpdateFunction([&](State *state)->Input {
			return function(dummy<Input>::input->getValue(state));
		});
	}
};

#endif //FUNCTION_RUNNER_H
