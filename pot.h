/***** pot.h *****/

#ifndef POT_H
#define POT_H

#include <component.h>
#include <vector>
#include <functional>

typedef std::function<void(BelaContext *, float, unsigned int, unsigned int, unsigned int)> pot_listener;

class pot : public activeComponent<float> {
public:
	/** pot constructor
	 * @analogPin - the index of the analogIn pin connected to the potentiometer
	 * @error - the least value delta that triggers a value update
	 * @minv - the minimum output value
	 * @maxv - the maximum output value
	 */
	pot(short analogPin, float error = 0.001, float minv = 0, float maxv = 1);
	
	// component interface
	void read(BelaContext *context, void *userData, unsigned int audioFrameCount, unsigned int analogFrameCount, unsigned int digitalFrameCount);

	
	/** set_range sets the output values range
	 * @minv - the minimum output value
	 * @maxv - the maximum output value
	 */
	void set_range(float minv, float maxv);
	
	/** set_error sets the minimum value error
	 * @error - the least value delta that triggers a value update
	 */
	void set_error(float error);
	
protected:
	short pin;
	
	bool inverse_reading;
	
	float minv;
	float maxv;
	float error;
	float value;
};

#endif //POT_H