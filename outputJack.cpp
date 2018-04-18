/***** outputJack.cpp *****/

#include <jack.h>

outputJack::outputJack(short analogPin):
	pin(analogPin),
	value(0)
{}

void outputJack::execute(BelaContext *context, void *userData, 
							unsigned int audioFrameCount, 
							unsigned int analogFrameCount, 
							unsigned int digitalFrameCount)
{
	analogWrite(context, analogFrameCount, pin, value);
}

void outputJack::setValue(float val)
{
	value = val;
}
