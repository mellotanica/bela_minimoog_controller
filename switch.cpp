/***** switch.cpp *****/

#include <switch.h>

switch_comp::switch_comp(short pin0, short pin2, switch_type type):
	pin0(pin0),
	pin2(pin2),
	state(0),
	type(type)
{}
	
void switch_comp::setup(BelaContext *context, void *userData)
{
	pinMode(context, 0, pin0, INPUT);
	pinMode(context, 0, pin2, INPUT);
}

void switch_comp::read(BelaContext *context, void *userData, 
						unsigned int audioFrameCount, 
						unsigned int analogFrameCount, 
						unsigned int digitalFrameCount)
{
	unsigned short tst = 1;
	bool tv0 = digitalRead(context, 0, pin0) != 0;
	bool tv2 = digitalRead(context, 0, pin2) != 0;
	
	switch(type) {
		case ON_OFF_ON:
			if (tv0) {
				tst = 0;
			} else if (tv2) {
				tst = 2;
			}
			break;
		case ON_BOTH_ON:
			if (tv0 && !tv2) {
				tst = 0;
			} else if (!tv0 && tv2) {
				tst = 2;
			}
			break;
	}
	
	if (state != tst) {
		state = tst;
		updateValue(context, state, audioFrameCount, analogFrameCount, digitalFrameCount);
	}
}

