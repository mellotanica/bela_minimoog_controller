#include <jack.h>

outputJack::outputJack(unsigned short pin):
	value(std::make_shared<Receiver<float>>()),
	pin(pin)
{}

void outputJack::render(State *state) 
{
	analogWrite(state->context, state->analogFrame, pin, value->getValue(state));
}
