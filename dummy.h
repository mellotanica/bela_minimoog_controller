#ifndef DUMMY_H
#define DUMMY_H

#include <component.h>

template <typename Input>
class dummy : public output {
public:
	dummy() : input(Receiver<Input>::make()) {}

	dummy(Emitter<Input> em) : input(Receiver<Input>::make(em)) {}

	virtual void render(State *execState) { input.getValue(state); }

	std::shared_ptr<Receiver<Input>> input;
}

#endif //DUMMY_H
