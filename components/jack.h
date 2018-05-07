#ifndef JACK_H
#define JACK_H

#include <base/component.h>

#include <components/constant.h>

class inputJack : public component {
public:
	inputJack(unsigned short analogPin);
	
	EmitterP<float> value;
};

class outputJack : public output {
public:
	outputJack(unsigned short analogPin,
			float multiplier = 1,
			EmitterP<float> rest_value = ZeroF);
	
	void render(State *state);
	void reset();

	ReceiverP<float> value;
protected:
	unsigned short pin;
	const float multiplier;

	EmitterP<float> rest_value;
};

#endif //JACK_H

