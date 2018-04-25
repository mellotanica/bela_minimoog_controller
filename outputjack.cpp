#include <components/jack.h>

outputJack::outputJack(unsigned short pin, std::shared_ptr<Emitter<float>> rest_value):
	value(Receiver<float>::make(rest_value)),
	pin(pin),
	rest_value(rest_value)
{}

void outputJack::reset()
{
	value->register_emitter(rest_value);
}

void outputJack::render(State *state) 
{
	analogWrite(state->context, state->analogFrame, pin, value->getValue(state));
}
