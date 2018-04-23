#include <components/jack.h>

outputJack::outputJack(unsigned short pin):
	value(Receiver<float>::make()),
	pin(pin)
{}

void outputJack::render(State *state) 
{
	analogWrite(state->context, state->analogFrame, pin, value->getValue(state));
}
