#include <component.h>

/** State - esecution state
 * @context - the execution context
 * @userData - user arbitrary data to pass to other render steps
 * @audioFrameCount - the reference audio frame
 * @analogFrameCount - the reference analog frame
 * @digitalFrameCount - the reference digital frame
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
		if(connected_emitter != nullptr) {
			return connected_emitter->getValue(state);
		}
		return defaultVal;
	}


	void register_emitter(Emitter<Value> *emitter)
	{
		connected_emitter = emitter;
	}

	void clear_emitter()
	{
		connected_emitter = nullptr;
	}

protected:
	Value defaultVal;
	Emitter<Value> * connected_emitter;
};

