/***** jack.h *****/

#ifndef JACK_H
#define JACK_H

#include <component.h>

class inputJack : public component {
public:
	inputJack(unsigned short analogPin);
	
	std::shared_ptr<Emitter<float>> value;
};

class outputJack : public output {
public:
	outputJack(unsigned short analogPin);
	
	void render(State *state);

	std::shared_ptr<Receiver<float>> value;
protected:
	unsigned short pin;
};

#endif //JACK_H

