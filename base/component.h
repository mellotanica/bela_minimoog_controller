#ifndef COMPONENT_H
#define COMPONENT_H

#include <base/net.h>

/** component interface for all components to automate the render routines
 */
class component {
public:
	/** setup called during the setup phase, implement with context-dependant component setup
	 * @context - the execution context
	 * @userData - user arbitrary data to pass to other render steps
	 */
	virtual void setup(BelaContext *context, void *userData) {};

	/** cleanup called during the cleanup phase, implement with context-dependant component cleanup
	 * @context - the execution context
	 * @userData - user arbitrary data to pass to other render steps
	 */
	virtual void cleanup(BelaContext *context, void *userData) {};

	/** reset should connect all component receivers to default emitters
	 */
	virtual void reset() {};
};

class output: public component {
public:
	/** render called during the main render phase should trigger evaluation chain
	 * state - the current execution state
	 */
	virtual void render(State *state) = 0;
};

#endif //COMPONENT_H
