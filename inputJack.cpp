/***** inputJack.cpp *****/

#include <jack.h>

inputJack::inputJack(short analogPin) :
	pin(analogPin)
{}

void inputJack::read(BelaContext *context, void *userData,
						unsigned int audioFrameCount, 
						unsigned int analogFrameCount, 
						unsigned int digitalFrameCount)
{
	float value = analogRead(context, analogFrameCount, pin);
	updateValue(context, value, audioFrameCount, analogFrameCount, digitalFrameCount);
}
