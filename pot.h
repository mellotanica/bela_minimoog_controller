/***** pot.h *****/

#ifndef POT_H
#define POT_H

#include <component.h>
#include <vector>
#include <functional>
#include <constant.h>

static constant<float> default_pot_error(0.001);
static constant<float> integer_pot_error(0.1);

class pot : public component {
public:
	/** pot constructor
	 * @analogPin - the index of the analogIn pin connected to the potentiometer
	 * @error - the least value delta that triggers a value update
	 * @minv - the minimum output value
	 * @maxv - the maximum output value
	 */
	pot(short analogPin, Emitter<float> * error = &default_pot_error);

	Receiver<float> minv;
	Receiver<float> maxv;
	Receiver<float> error;
	Emitter<float> value;
protected:
	short pin;

	float readVal(State *execState);
};

#endif //POT_H
