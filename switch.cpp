/***** switch.cpp *****/

#include <switch.h>

switch_comp::switch_comp(short pin0, short pin2, switch_type type):
	position(std::make_shared<Emitter<unsigned short>>()),
	pin0(pin0),
	pin2(pin2),
	type(type)
{
	position->setUpdateFunction([&](State *execState)->unsigned short {
		return this->readPosition(execState);
	});
}
	
void switch_comp::setup(BelaContext *context, void *userData)
{
	pinMode(context, 0, pin0, INPUT);
	pinMode(context, 0, pin2, INPUT);
}

unsigned short switch_comp::readPosition(State *execState)
{
	unsigned short tst = 1;
	bool tv0 = digitalRead(execState->context, 0, pin0) != 0;
	bool tv2 = digitalRead(execState->context, 0, pin2) != 0;
	
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
	return tst;
}

