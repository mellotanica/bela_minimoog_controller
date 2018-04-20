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

	static std::shared_ptr<constant<Value>> make(Value val)
	{
		return std::make_shared<constant<Value>>(val);
	}
};

static auto True = std::make_shared<constant<bool>>(true);
static auto False = std::make_shared<constant<bool>>(false);

static auto OneI = std::make_shared<constant<int>>(1);
static auto ZeroI = std::make_shared<constant<int>>(0);

static auto OneF = std::make_shared<constant<float>>(1);
static auto ZeroF = std::make_shared<constant<float>>(0);

#endif
