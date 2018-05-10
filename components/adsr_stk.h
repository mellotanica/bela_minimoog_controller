#ifndef ADSR_STK_H
#define ADSR_STK_H

#include <base/component.h>

#include <components/constant.h>
#include <components/adsr.h>

#include <stk/ADSR.h>

class adsr_stk : public component {
public:
	adsr_stk(EmitterP<float> attack = adsr_default_time,
		EmitterP<float> decay = adsr_default_time,
		EmitterP<float> sustain = adsr_default_sustain,
		EmitterP<float> release = adsr_default_time,
		EmitterP<float> attack_level = OneF):
			attack(Receiver<float>::make(attack)),
			decay(Receiver<float>::make(decay)),
			sustain(Receiver<float>::make(sustain)),
			release(Receiver<float>::make(release)),
			attack_level(Receiver<float>::make(attack_level)),
			gate(Receiver<bool>::make(False)),
			value(Emitter<float>::make()),
			inactive(Emitter<bool>::make())
	{
		inactive->setUpdateFunction([&](State *state)->bool {
			return this->impl.getState() == stk::ADSR::IDLE;
		});

		value->setUpdateFunction([&](State *state)->float {
			this->impl.setAttackTime(this->attack->getValue(state));
			this->impl.setAttackTarget(this->attack_level->getValue(state));
			this->impl.setDecayTime(this->decay->getValue(state));
			this->impl.setSustainLevel(this->sustain->getValue(state));
			this->impl.setReleaseTime(this->release->getValue(state));
			
			if(this->gate->getValue(state) != this->gate->getPreviousValue()){
				if(this->gate->getLastValue()){
					this->impl.keyOn();
				} else {
					this->impl.keyOff();
				}
			}
			
			return this->impl.tick();
		});
	}

	void reset()
	{
		attack->register_emitter(adsr_default_time);
		decay->register_emitter(adsr_default_time);
		release->register_emitter(adsr_default_time);
		sustain->register_emitter(adsr_default_sustain);
		attack_level->register_emitter(OneF);
		gate->register_emitter(False);
	}

	ReceiverP<float> attack;
	ReceiverP<float> decay;
	ReceiverP<float> sustain;
	ReceiverP<float> release;
	ReceiverP<float> attack_level;

	ReceiverP<bool> gate;

	EmitterP<float> value;
	EmitterP<bool> inactive;
protected:
	stk::ADSR impl;
};

#endif //ADSR_STK_H
