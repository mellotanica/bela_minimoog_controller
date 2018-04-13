/***** killswitch.h *****/

#ifndef KILLSWITCH_H
#define KILLSWITCH_H

#include <component.h>
#include <functional>

class killswitch : public activeComponent<bool> {
public:
	/** killswitch constructor
	 * @pin - the digital pin connected to the killswitch expressed with digital_gpio_mapping.h naming
	 * @defaultState - the default state of the switch (true - NC / false - NO)
	 * @ debounceMsecs - debounce time in milliseconds (0 to disable debounce)
	 */
	killswitch(short pin, bool defaultState, unsigned int debounceMsecs = 2);
	
	// component interface
	void setup(BelaContext *context, void *userData);
	void read(BelaContext *context, void *userData, unsigned int audioFrameCount, unsigned int analogFrameCount, unsigned int digitalFrameCount);

protected:
	short pin;
	bool defaultState;
	bool state;
	
	unsigned int debounceMsecs;
	unsigned int debounceTime;
	unsigned int debounceCounter;
	bool debouncing;
};

#endif //KILLSWITCH_H
