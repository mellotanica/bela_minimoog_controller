#ifndef POT_H
#define POT_H

#include <base/component.h>

#include <vector>
#include <functional>

#include <components/constant.h>

static auto default_pot_error = constant<float>::make(0.0005);
static auto integer_pot_error = constant<float>::make(0.1);

class pot : public component {
public:
	/** pot constructor
	 * @analogPin - the index of the analogIn pin connected to the potentiometer
	 * @error - the least value delta that triggers a value update
	 * @minv - the minimum output value
	 * @maxv - the maximum output value
	 */
	pot(short analogPin, 
		EmitterP<float> error = default_pot_error,
		EmitterP<float> minv = ZeroF,
		EmitterP<float> maxv = OneF);

	void reset();

	ReceiverP<float> minv;
	ReceiverP<float> maxv;
	ReceiverP<float> error;

	EmitterP<float> value;
protected:
	short pin;

	float readVal(State *execState);
};

#endif //POT_H
