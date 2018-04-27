#ifndef MIXER_H
#define MIXER_H

#include <base/component.h>

#include <components/constant.h>

#include <vector>

enum mixer_type
{
	ADD,
	MUL,
	OR,
	AND
};

static auto mixer_max_modeF = constant<float>::make(MUL+1);

static auto mixer_add = constant<mixer_type>::make(ADD);
static auto mixer_mul = constant<mixer_type>::make(MUL);
static auto mixer_or = constant<mixer_type>::make(OR);
static auto mixer_and = constant<mixer_type>::make(AND);

template <typename Value>
class mixer : public component {
public:
	mixer(std::shared_ptr<Emitter<mixer_type>> mix_mode = mixer_add):
		mix_mode(Receiver<mixer_type>::make(mix_mode)),
		output(Emitter<Value>::make())
	{
		output->setUpdateFunction([&](State *state)->Value{
			return this->mix(state);
		});
	}

	void register_input(std::shared_ptr<Emitter<Value>> em)
	{
		inputs.push_back(Receiver<Value>::make(em));
	}

	void clear_inputs()
	{
		inputs.clear();
	}

	std::shared_ptr<Receiver<mixer_type>> mix_mode;

	std::shared_ptr<Emitter<Value>> output;

protected:
	std::vector<std::shared_ptr<Receiver<Value>>> inputs;

	inline Value mix(State *state){
		Value v = 0;
		if(inputs.size() > 0) {
			switch(mix_mode->getValue(state)){
				case ADD:
				case OR:
					for (auto i: inputs) {
						v += i->getValue(state);
					}
					break;
				case MUL:
				case AND:
					v = 1;
					for (auto i: inputs) {
						v *= i->getValue(state);
					}
					break;
			}
		}
		return v;
	}
};

#endif //MIXER_H
