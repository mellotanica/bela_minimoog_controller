/***** jack.h *****/

#ifndef JACK_H
#define JACK_H

#include <component.h>

class inputJack : public component {
public:
	inputJack(unsigned short analogPin);
	
	Emitter<float> value;
};

class outputJack : public output {
public:
	outputJack(unsigned short analogPin);
	
	void render(State *state);

	Receiver<float> value;
protected:
	unsigned short pin;
};

#endif //JACK_H

