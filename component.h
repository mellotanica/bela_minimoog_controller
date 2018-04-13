/***** component.h *****/

#ifndef COMPONENT_H
#define COMPONENT_H

#include <Bela.h>
#include <vector>
#include <functional>

/** component interface for all components to automate the render routines
 */
class component {
public:
	component(bool active = false): active_state(active) {}

	/** setup called during the setup phase, implement with context-dependant component setup
	 * @context - the execution context
	 * @userData - user arbitrary data to pass to other render steps
	 */
	virtual void setup(BelaContext *context, void *userData) {};

	/** read called during the render phase before execute, input components should implement this method
	 * @context - the execution context
	 * @userData - user arbitrary data to pass to other render steps
	 * @audioFrameCount - the reference audio frame
	 * @analogFrameCount - the reference analog frame
	 * @digitalFrameCount - the reference digital frame
	 */
	virtual void read(BelaContext *context, void *userData, 
						unsigned int audioFrameCount, 
						unsigned int analogFrameCount, 
						unsigned int digitalFrameCount) {};
	
	/** execute called during the render phase after read, output components should implement this method
	 * @context - the execution context
	 * @userData - user arbitrary data to pass to other render steps
	 * @audioFrameCount - the reference audio frame
	 * @analogFrameCount - the reference analog frame
	 * @digitalFrameCount - the reference digital frame
	 */
	virtual void execute(BelaContext *context, void *userData, 
						unsigned int audioFrameCount, 
						unsigned int analogFrameCount, 
						unsigned int digitalFrameCount) {};
	
	/** cleanup called during the cleanup phase, implement with context-dependant component cleanup
	 * @context - the execution context
	 * @userData - user arbitrary data to pass to other render steps
	 */
	virtual void cleanup(BelaContext *context, void *userData) {};
	
	/** set_active enables and disables the component
	 * @active - the new state of the component
	 */
	virtual void set_active(bool active) { rt_printf("setting active component: %d (prev: %d)\n", active, active_state); active_state = active; }
	
	virtual bool is_active() { return active_state; }
	
protected:
	bool active_state = false;
};

template<typename... Values> class activeComponent: public component {
public:
	/** register_listener registers a listener for this potentiometer value change events
	 * @function - will be called each time the value changes exceeding the error
	 */
	void register_listener(std::function<void(BelaContext *, Values..., unsigned int , unsigned int, unsigned int)> function) 
			{ listeners.push_back(function); }
	
	/** clear_listeners removes all registered listeners
	 */
	void clear_listeners() { listeners.clear(); }
protected:
	std::vector<std::function<void(BelaContext *, Values..., unsigned int , unsigned int, unsigned int)>> listeners;
	
	void updateValue(BelaContext *context, Values... values, unsigned int audioFrame, unsigned int analogFrame, unsigned int digitalFrame) 
			{ for(auto l : listeners) l(context, values..., audioFrame, analogFrame, digitalFrame); }
};

#endif //COMPONENT_H