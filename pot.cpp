#include <components/pot.h>

#include <math_neon.h>
#include <utility>

#define POT_MAX_READ 0.83

pot::pot(short analogPin, 
		EmitterP<float> def_error,
		EmitterP<float> def_minv,
		EmitterP<float> def_maxv) :
	minv(Receiver<float>::make(def_minv)),
	maxv(Receiver<float>::make(def_maxv)),
	error(Receiver<float>::make(def_error)),
	value(Emitter<float>::make()),
	pin(analogPin)
{
	value->setUpdateFunction([&](State *execState)->float {
		return this->readVal(execState);
	});
}

void pot::reset()
{
	minv->register_emitter(ZeroF);
	maxv->register_emitter(OneF);
	error->register_emitter(default_pot_error);
}

float pot::readVal(State *execState)
{
	float mnv = minv->getValue(execState);
	float mxv = maxv->getValue(execState);
	bool inverse_reading = true;
	if(mnv > mxv) {
		std::swap(mnv, mxv);
		inverse_reading = false;
	}

	float val = map(analogRead(execState->context, 
				execState->analogFrame,
				pin), 
			0, 
			POT_MAX_READ, 
			mnv,
			mxv);
	// limit value if exceeding range
	if (val < mnv) {
		val = mnv;
	} else if (val > mxv) {
		val = mxv;
	}
	
	if (inverse_reading) {
		val = (mxv - val) + mnv;
	}
	
	// chenge the actual value only if the difference with read data is at least the error
	if (fabsf_neon(val - value->getLastValue()) > error->getValue(execState)) {
		return val;
	} else {
		return value->getLastValue();
	}
}

