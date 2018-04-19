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
};

static constant<bool> True(true);
static constant<bool> False(false);

static constant<int> oneI(1);
static constant<int> zeroI(0);

static constant<float> oneF(1);
static constant<float> zeroF(0);

#endif
