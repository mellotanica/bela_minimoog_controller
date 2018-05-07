#ifndef DUMMY_H
#define DUMMY_H

#include <base/component.h>

template <typename Input>
class dummy : public output {
public:
	dummy() : input(Receiver<Input>::make()) {}

	dummy(EmitterP<Input> em) : input(Receiver<Input>::make(em)) {}

	virtual void render(State *state) { input->getValue(state); }

	ReceiverP<Input> input;
};

#endif //DUMMY_H
