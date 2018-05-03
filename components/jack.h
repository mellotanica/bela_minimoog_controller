#ifndef JACK_H
#define JACK_H

#include <base/component.h>

#include <components/constant.h>

class inputJack : public component {
public:
	inputJack(unsigned short analogPin);
	
	std::shared_ptr<Emitter<float>> value;
};

class outputJack : public output {
public:
	outputJack(unsigned short analogPin,
			float multiplier = 1,
			std::shared_ptr<Emitter<float>> rest_value = ZeroF);
	
	void render(State *state);
	void reset();

	std::shared_ptr<Receiver<float>> value;
protected:
	unsigned short pin;
	const float multiplier;

	std::shared_ptr<Emitter<float>> rest_value;
};

#endif //JACK_H

