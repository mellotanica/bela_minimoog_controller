/***** component.h *****/

#ifndef COMPONENT_H
#define COMPONENT_H

#include <Bela.h>

/** component interface for all components to automate the render routines
 */
class component {
public:
	/** setup called during the setup phase, implement with context-dependant component setup
	 * @context - the execution context
	 * @userData - user arbitrary data to pass to other render steps
	 */
	virtual void setup(BelaContext *context, void *userData) = 0;

	/** read called during the render phase before execute, input components should implement this method
	 * @context - the execution context
	 * @userData - user arbitrary data to pass to other render steps
	 * @audioFrameCount - the reference audio frame
	 * @analogFrameCount - the reference analog frame
	 * @digitalFrameCount - the reference digital frame
	 */
	virtual void read(BelaContext *context, void *userData, unsigned int audioFrameCount, unsigned int analogFrameCount, unsigned int digitalFrameCount) = 0;
	
	/** execute called during the render phase after read, output components should implement this method
	 * @context - the execution context
	 * @userData - user arbitrary data to pass to other render steps
	 * @audioFrameCount - the reference audio frame
	 * @analogFrameCount - the reference analog frame
	 * @digitalFrameCount - the reference digital frame
	 */
	virtual void execute(BelaContext *context, void *userData, unsigned int audioFrameCount, unsigned int analogFrameCount, unsigned int digitalFrameCount) = 0;
	
	/** cleanup called during the cleanup phase, implement with context-dependant component cleanup
	 * @context - the execution context
	 * @userData - user arbitrary data to pass to other render steps
	 */
	virtual void cleanup(BelaContext *context, void *userData) = 0;
};

#endif //COMPONENT_H