#include <jack.h>

outputJack::outputJack(unsigned short pin):
	pin(pin)
{}

void outputJack::render(State *state) 
{
	analogWrite(state->context, state->analogFrameCount, pin, value.getValue(state));
}
