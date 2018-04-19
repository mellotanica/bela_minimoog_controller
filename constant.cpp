#include <constant.h>

constant::constant(Value val) :
	value(val)
{
	emitters.push_back(&value);
	value.setUpdateFunction([=](State *state)->Value{
		return val;		
	});
}
