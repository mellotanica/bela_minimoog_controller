#ifndef TIMER_H
#define TIMER_H

#include <base/component.h>

#include <components/constant.h>

class timer : public component {
public:
	timer(EmitterP<float> duration = OneF,
		EmitterP<bool> oneShot = True);

	ReceiverP<float> duration;
	ReceiverP<bool> oneShot;
	
	ReceiverP<bool> reset;

	EmitterP<bool> trigger;
	EmitterP<bool> running;
private:
	bool run;

	unsigned long frames_duration;
	unsigned long step;
};

#endif //TIMER_H
