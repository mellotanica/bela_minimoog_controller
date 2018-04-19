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

enum mixing_mode
{
	ADD = 0,
	MULTIPLY,
};

template <typename Value>
class SimpleReceiver {
public:
	SimpleReceiver(Value default_val = 0):
		defaultVal(default_val)
	{
		update_mixing_mode([](Value a, Value b)->Value {
			return b;
		});
	}

	Value getValue(State *state)
	{
		Value val = defaultVal;
		for(auto e : sources) {
			val = mixer(val, e->getValue(state));
		}
		return val;
	}


	void register_emitter(Emitter<Value> *emitter)
	{
		sources.push_back(emitter);
	}

	void clear_emitters()
	{ 
		sources.clear(); 
	}

protected:
	Value defaultVal;
	std::function<Value(Value, Value)> mixer;
	std::vector<Emitter<Value> *> sources;

	void update_mixing_mode(std::function<Value(Value, Value)> mfn)
	{
		mixer = mfn;
	}
};

template<typename Value> 
class Receiver : public SimpleReceiver<Value> {
public:
	Receiver(Value default_val = 0, mixing_mode mix_mode = ADD):
		SimpleReceiver<Value>(default_val)
	{
		set_mixing_mode(mix_mode);
	}

	void set_mixing_mode(mixing_mode mix_mode)
	{
		switch(mix_mode){
			case ADD:
			default:
				SimpleReceiver<Value>::update_mixing_mode([](Value a, Value b)->Value {
					return a + b;
				});
				break;
			case MULTIPLY:
				SimpleReceiver<Value>::update_mixing_mode([](Value a, Value b)->Value {
					return a * b;
				});
				break;
		}
	}

};

