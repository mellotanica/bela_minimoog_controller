#ifndef NET_H
#define NET_H

#include <Bela.h>

#include <functional>
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
	uint64_t totalFramesElapsed;
	float inverse_sample_rate;
} State;

template<typename Value> 
class Emitter {
public:
	Emitter():
		lastElapsed(0)
	{}

	Emitter(Value defaultVal):
		lastElapsed(0),
		lastValidValue(defaultVal)
	{}

	Value getValue(State *state)
	{
		if (state->totalFramesElapsed != lastElapsed){
			lastElapsed = state->totalFramesElapsed;
			if (updateFn == NULL) {
				rt_printf("ERROR: Emitter %s has been started without update function\n", __PRETTY_FUNCTION__);
			} else {
				Value nv = updateFn(state);
				if(nv != lastValidValue) {
					lastValidValue = nv;
				}
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

	static inline std::shared_ptr<Emitter<Value>> make()
	{
		return std::make_shared<Emitter<Value>>();
	}

	static inline std::shared_ptr<Emitter<Value>> make(Value defaultVal)
	{
		return std::make_shared<Emitter<Value>>(defaultVal);
	}

protected:
	uint64_t lastElapsed;
	Value lastValidValue;
	std::function<Value(State *)> updateFn;
};

template <typename Value>
class Receiver {
public:
	Receiver()
	{
		clear_emitter();
	}

	Receiver(Value defaultVal):
		defaultVal(defaultVal)
	{
		clear_emitter();
	}

	Receiver(std::shared_ptr<Emitter<Value>> emitter)
	{
		register_emitter(emitter);
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

	static inline std::shared_ptr<Receiver<Value>> make()
	{
		return std::make_shared<Receiver<Value>>();
	}

	static inline std::shared_ptr<Receiver<Value>> make(Value defaultVal)
	{
		return std::make_shared<Receiver<Value>>(defaultVal);
	}

	static inline std::shared_ptr<Receiver<Value>> make(std::shared_ptr<Emitter<Value>> emitter)
	{
		return std::make_shared<Receiver<Value>>(emitter);
	}

protected:
	Value defaultVal;
	std::shared_ptr<Emitter<Value>> connected_emitter;
};

template <typename Value>
using EmitterP = std::shared_ptr<Emitter<Value>>;

template <typename Value>
using ReceiverP = std::shared_ptr<Receiver<Value>>;

#endif //NET_H
