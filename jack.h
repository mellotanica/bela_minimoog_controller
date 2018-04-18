/***** jack.h *****/

#ifndef JACK_H
#define JACK_H

#include <component.h>

class inputJack : public activeComponent<float> {
public:
	inputJack(short analogPin);
	
	void read(BelaContext *context, void *userData,
						unsigned int audioFrameCount, 
						unsigned int analogFrameCount, 
						unsigned int digitalFrameCount);
protected:
	short pin;
};

class outputJack : public component {
public:
	outputJack(short analogPin);
	
	void execute(BelaContext *context, void *userData, 
						unsigned int audioFrameCount, 
						unsigned int analogFrameCount, 
						unsigned int digitalFrameCount);

	void setValue(float val);
	
protected:
	short pin;
	
	float value;
};

#endif //JACK_H
