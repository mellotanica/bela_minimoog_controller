#ifndef CONSTANT_H
#define CONSTANT_H

#include <component.h>

template <typename Value> 
class constant : public Emitter<Value> {
public:
	constant(Value val):
		Emitter<Value>(val)
	{
		Emitter<Value>::setUpdateFunction([=](State *state)->Value{
			return val;		
		});
	}

	static inline std::shared_ptr<constant<Value>> make(Value val)
	{
		return std::make_shared<constant<Value>>(val);
	}
};

static auto True = constant<bool>::make(true);
static auto False = constant<bool>::make(false);

static auto OneI = constant<int>::make(1);
static auto ZeroI = constant<int>::make(0);

static auto OneF = constant<float>::make(1);
static auto ZeroF = constant<float>::make(0);

#endif
