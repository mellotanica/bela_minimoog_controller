#include <components/jack.h>

outputJack::outputJack(unsigned short pin, float multiplier, EmitterP<float> rest_value):
	value(Receiver<float>::make(rest_value)),
	pin(pin),
	multiplier(multiplier),
	rest_value(rest_value)
{}

void outputJack::reset()
{
	value->register_emitter(rest_value);
}

void outputJack::render(State *state) 
{
	// truncate input to 1 before scaling
	float v = value->getValue(state);
	bool g = v > 1;
	v = g + ((1-g) * v);

	// output scaled value
	analogWrite(state->context, state->analogFrame, pin, multiplier * v);
}
