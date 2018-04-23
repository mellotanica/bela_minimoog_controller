#ifndef SWITCH_H
#define SWITCH_H

#include <base/component.h>

enum switch_type 
{
	ON_OFF_ON,
	ON_BOTH_ON,
};

class switch_comp : public component {
public:
	/** switch_comp constructor
	 * @pin0 - the pin generating value 0 expressed with digital_gpio_mapping.h naming
	 * @pin2 - the pin generating value 2 expressed with digital_gpio_mapping.h naming
	 * @type - the switch type (ON-OFF-ON / ON-BOTH-ON)
	 */
	switch_comp(short pin0, short pin2, switch_type type);
	
	// component interface
	void setup(BelaContext *context, void *userData);

	std::shared_ptr<Emitter<unsigned short>> position;
protected:
	short pin0;
	short pin2;
	switch_type type;

	unsigned short readPosition(State *execState);
};

#endif //SWITCH_H
