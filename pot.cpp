/***** pot.cpp *****/

#include <pot.h>
#include <math_neon.h>
#include <utility>

pot::pot(short analogPin, 
		std::shared_ptr<Emitter<float>> def_error,
		std::shared_ptr<Emitter<float>> def_minv,
		std::shared_ptr<Emitter<float>> def_maxv) :
	minv(std::make_shared<Receiver<float>>(def_minv)),
	maxv(std::make_shared<Receiver<float>>(def_maxv)),
	error(std::make_shared<Receiver<float>>(def_error)),
	value(std::make_shared<Emitter<float>>()),
	pin(analogPin)
{
	value->setUpdateFunction([&](State *execState)->float {
		return this->readVal(execState);
	});
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
			0.83, 
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

