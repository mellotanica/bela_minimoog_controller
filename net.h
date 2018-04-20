#ifndef NET_H
#define NET_H

#include <functional>
#include <Bela.h>
#include <memory>

/** State - esecution state
 * @context - the execution context
 * @userData - user arbitrary data to pass to other render steps
 * @audioFrame - the reference audio frame
 * @analogFrame - the reference analog frame
 * @digitalFrame - the reference digital frame
 */
typedef struct {
	BelaContext *context;
	void *userData;
	unsigned int audioFrame;
	unsigned int analogFrame;
	unsigned int digitalFrame;
} State;

template<typename Value> 
class Emitter {
public:
	Emitter(Value default_val = 0):
		lastElapsed(0),
		lastValidValue(default_val),
		updateFn(NULL)
	{}

	Value getValue(State *state)
	{
		if (state->context->audioFramesElapsed != lastElapsed){
			lastElapsed = state->context->audioFramesElapsed;
			if (updateFn == NULL) {
				rt_printf("ERROR: emitter has been started without update function\n");
			} else {
				lastValidValue = updateFn(state);
			}
		}
		return lastValidValue;
	}

	Value getLastValue()
	{
		return lastValidValue;
	}

	void setUpdateFunction(std::function<Value(State *)> fn)
	{
		updateFn = fn;
	}

protected:
	uint64_t lastElapsed;
	Value lastValidValue;
	std::function<Value(State *)> updateFn;
};

template <typename Value>
class Receiver {
public:
	Receiver(Value default_val = 0):
		defaultVal(default_val)
	{
		clear_emitter();
	}

	Value getValue(State *state)
	{
		if(connected_emitter) {
			return connected_emitter->getValue(state);
		}
		return defaultVal;
	}


	void register_emitter(std::shared_ptr<Emitter<Value>> emitter)
	{
		connected_emitter = emitter;
	}

	void clear_emitter()
	{
		connected_emitter.reset();
	}

protected:
	Value defaultVal;
	std::shared_ptr<Emitter<Value>> connected_emitter;
};

#endif //NET_H
