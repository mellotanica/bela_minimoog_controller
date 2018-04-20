#ifndef VARIABLE_H
#define VARIABLE_H

#include <component.h>

template <typename Value>
class variable : public component {
public:
	variable(Value val):
		value(std::make_shared<Emitter<Value>>(val))
	{
		value->setUpdateFunction([&](State *state)->Value{
			return this->innerValue;
		});
	}

	void set(Value v)
	{
		innerValue = v;
	}

	std::shared_ptr<Emitter<Value>> value;
protected:
	Value innerValue;
}


#endif //VARIABLE_H
