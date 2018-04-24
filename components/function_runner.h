#ifndef FUNCTION_RUNNER_H
#define FUNCTION_RUNNER_H

#include <components/dummy.h>

#include <functional>

template <typename Input>
class function_runner : public dummy<Input> {
public:
	function_runner(std::function<void(Input)> function):
		dummy<Input>(),
		function(function)
	{}

	function_runner(std::function<void(Input)> function, Emitter<Input> em):
		dummy<Input>(em),
		function(function)
	{}

	virtual void render(State *state) { function(dummy<Input>::input->getValue(state)); }

protected:
	std::function<void(Input)> function;
};

#endif //FUNCTION_RUNNER_H
