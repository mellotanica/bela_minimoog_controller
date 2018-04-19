/***** pot.cpp *****/

#include <pot.h>
#include <math_neon.h>
#include <utility>

pot::pot(short analogPin, Emitter<float> *def_error) :
	pin(analogPin)
{
	error.register_emitter(def_error);
	value.setUpdateFunction([&](State *execState)->float {
		return this->readVal(execState);
	});
}

void pot::set_error(Emitter<float> *error_em) 
{
	error.clear_emitters();
	error.register_emitter(error_em);
}

float pot::readVal(State *execState)
{
	float mnv = minv.getValue(execState);
	float mxv = maxv.getValue(execState);
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
		val = mxv - val;
	}
	
	// chenge the actual value only if the difference with read data is at least the error
	if (fabsf_neon(val - value.getLastValue()) > error.getValue(execState)) {
		return val;
	} else {
		return value.getLastValue();
	}
}

