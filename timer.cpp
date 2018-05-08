#include <components/timer.h>

timer::timer(EmitterP<float> duration,
		EmitterP<bool> oneShot):
	duration(Receiver<float>::make(duration)),
	oneShot(Receiver<bool>::make(oneShot)),
	reset(Receiver<bool>::make(False)),
	trigger(Emitter<bool>::make()),
	running(Emitter<bool>::make()),
	run(false),
	frames_duration((unsigned long) -1),
	step(0)
{
	trigger->setUpdateFunction([&](State *state)->bool {
		this->frames_duration = this->duration->getValue(state) * 
					state->context->audioSampleRate;

		bool ret = (this->step >= this->frames_duration);

		if(ret || this->reset->getValue(state)) {
			this->step = 0;
			this->run = !this->oneShot->getValue(state) || 
					this->reset->getValue(state);
		} else {
			this->step += (unsigned) this->run;
		}

		return ret;
	});

	running->setUpdateFunction([&](State *state)->bool {
		return this->run;
	});
}
