#ifndef VARIABLE_H
#define VARIABLE_H

#include <component.h>
#include <vector>

template <typename Value>
class variable : public component {
public:
	variable():
		value(Emitter<Value>::make())
	{
		init_emitter();
	}

	variable(Value val):
		value(Emitter<Value>::make(val))
	{
		init_emitter();
	}

	virtual void set(Value v)
	{
		innerValue = v;
	}

	virtual Value get()
	{
		return innerValue;
	}

	std::shared_ptr<Emitter<Value>> value;
protected:
	Value innerValue;
	
private:
	void init_emitter()
	{
		value->setUpdateFunction([&](State *state)->Value{
			return this->innerValue;
		});
	}
};

template <typename Value>
class variableVector : public variable<std::vector<Value>> {
public:	
	variableVector():
		variable<std::vector<Value>>(),
		back(Emitter<Value>::make()),
		front(Emitter<Value>::make()),
		size(Emitter<size_t>::make())
	{
		back->setUpdateFunction([&](State *state)->Value{
			return this->getBack();
		});

		front->setUpdateFunction([&](State *state)->Value{
			return this->getFront();
		});
		
		size->setUpdateFunction([&](State *state)->size_t{
			return this->getSize();
		});
	}

	void set(std::vector<Value> v) 
	{
		variable<std::vector<Value>>::innerValue.swap(v);
	}

	void push_back(Value v) 
	{
		variable<std::vector<Value>>::innerValue.push_back(v);
	}

	void push_front(Value v) 
	{
		variable<std::vector<Value>>::innerValue.insert(
			variable<std::vector<Value>>::innerValue.begin(), v);
	}

	size_t getSize()
	{
		return variable<std::vector<Value>>::innerValue.size();
	}

	Value getBack()
	{
		return variable<std::vector<Value>>::innerValue.back();
	}

	Value getFront()
	{
		return variable<std::vector<Value>>::innerValue.front();
	}

	std::shared_ptr<Emitter<Value>> back;
	std::shared_ptr<Emitter<Value>> front;
	std::shared_ptr<Emitter<size_t>> size;
};

#endif //VARIABLE_H
