/***** pot.h *****/

#ifndef POT_H
#define POT_H

#include <component.h>
#include <vector>
#include <functional>

typedef std::function<void(BelaContext *, float, unsigned int, unsigned int, unsigned int)> pot_listener;

class pot : public component {
public:
	/** pot constructor
	 * @analogPin - the index of the analogIn pin connected to the potentiometer
	 * @error - the least value delta that triggers a value update
	 * @minv - the minimum output value
	 * @maxv - the maximum output value
	 */
	pot(short analogPin, float error = 0.001, float minv = 0, float maxv = 1);
	
	// component interface
	void setup(BelaContext *context, void *userData);
	void read(BelaContext *context, void *userData, unsigned int audioFrameCount, unsigned int analogFrameCount, unsigned int digitalFrameCount);
	void execute(BelaContext *context, void *userData, unsigned int audioFrameCount, unsigned int analogFrameCount, unsigned int digitalFrameCount);
	void cleanup(BelaContext *context, void *userData);
	
	/** set_active enables and disables the led
	 * @active - the new state of the led
	 */
	void set_active(bool active);
	
	/** set_range sets the output values range
	 * @minv - the minimum output value
	 * @maxv - the maximum output value
	 */
	void set_range(float minv, float maxv);
	
	/** set_error sets the minimum value error
	 * @error - the least value delta that triggers a value update
	 */
	void set_error(float error);
	
	
	/** register_listener registers a listener for this potentiometer value change events
	 * @function - will be called each time the value changes exceeding the error
	 */
	void register_listener(pot_listener function);
	
	/** clear_listeners removes all registered listeners
	 */
	void clear_listeners();
	
protected:
	short pin;
	
	bool is_active;
	
	float minv;
	float maxv;
	float error;
	float value;
	
	std::vector<pot_listener> listeners;
};

#endif //POT_H