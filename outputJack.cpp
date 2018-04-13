/***** outputJack.cpp *****/

#include <jack.h>

outputJack::outputJack(short analogPin):
	pin(analogPin),
	lastvalue(-1)
{}
	
void outputJack::setup(BelaContext *context, void *userData)
{
	buffersize = context->analogFrames;
	buffer = new float[buffersize];
}

void outputJack::execute(BelaContext *context, void *userData, 
							unsigned int audioFrameCount, 
							unsigned int analogFrameCount, 
							unsigned int digitalFrameCount)
{
	float val = buffer[analogFrameCount];
	if(val != lastvalue) {
		analogWrite(context, analogFrameCount, pin, val);
		lastvalue = val;
	}
}

void outputJack::cleanup(BelaContext *context, void *userData)
{
	delete[] buffer;
}
	
void outputJack::writeFrame(float v, unsigned int n)
{
	if(n < buffersize) {
		buffer[n] = v;
	}
}
