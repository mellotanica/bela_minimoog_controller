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
	
	void setup(BelaContext *context, void *userData);
	void execute(BelaContext *context, void *userData, 
						unsigned int audioFrameCount, 
						unsigned int analogFrameCount, 
						unsigned int digitalFrameCount);
	void cleanup(BelaContext *context, void *userData);
	
	void writeFrame(float v, unsigned int n);
	
	unsigned int getBufferSize() { return buffersize; }
	
protected:
	short pin;
	
	float lastvalue;
	float *buffer;
	unsigned int buffersize;
};

#endif //JACK_H
