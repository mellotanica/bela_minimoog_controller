#ifndef VARIABLE_H
#define VARIABLE_H

#include <component.h>

template <typename Value>
class variable : public component {
public:
	variable(Value val):
		value(val)
	{
		value.setUpdateFunction([&](State *state)->Value{
			return this->innerValue;
		});
	}

	Emitter<Value> value;
protected:
	Value innerValue;
}


#endif //VARIABLE_H
