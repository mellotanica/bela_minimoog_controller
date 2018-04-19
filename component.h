/***** component.h *****/

#ifndef COMPONENT_H
#define COMPONENT_H

#include <Bela.h>
#include <vector>
#include <functional>
#include <net.h>

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

	/** cleanup called during the cleanup phase, implement with context-dependant component cleanup
	 * @context - the execution context
	 * @userData - user arbitrary data to pass to other render steps
	 */
	virtual void cleanup(BelaContext *context, void *userData) {};
	
	/** set_active enables and disables the component
	 * @active - the new state of the component
	 */
	virtual void set_active(bool active) { active_state = active; }
	
	virtual bool is_active() { return active_state; }
	
protected:
	bool active_state = false;
};

class output: public component {
public:
	/** render called during the main render phase should trigger evaluation chain
	 * state - the current execution state
	 */
	virtual void render(State *state) = 0;
};

/*
class componentsHandler {
public:
	static componentsHandler& getInstance()
	{
		static componentsHandler instance;
		return instance;
	}
	
	void register_component(component *comp) { components.push_back(comp); }
	//TODO
	//void unregister_component(component *comp) {}
	void clear_components() { components.clear(); }
	
	// disable copy and assignement operators
	componentsHandler(componentsHandler const&) = delete;
    void operator=(componentsHandler const&) = delete;
private:
	static componentsHandler *instance;
	std::vector<component *> components;
	
	componentsHandler() {}
};

componentsHandler::instance = NULL;
*/

#endif //COMPONENT_H
