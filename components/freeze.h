#ifndef FREEZE_H
#define FREEZE_H

#include <base/component.h>

const short freeze_max_buffer_size = 4;

template <typename Value>
class freeze : public component {
public:
	freeze(EmitterP<Value> input = nullptr,
		EmitterP<bool> engage = False):
		input(Receiver<Value>::make(input)),
		engage(Receiver<bool>::make(engage)),
		output(Emitter<Value>::make()),
		next(0)
	{
		for(int i = 0; i < freeze_max_buffer_size; i++) {
			buffer[i] = 0;
		}

		output->setUpdateFunction([&](State *state)->Value {
			if(! this->engage->getValue(state)){
				Value val = this->input->getValue(state);
				this->buffer[this->next] = val;
				this->next = (this->next + 1) % freeze_max_buffer_size;
				return val;
			} else {
				return this->buffer[this->next];
			}
		});
	}

	ReceiverP <Value> input;
	ReceiverP <bool> engage;

	EmitterP <Value> output;
private:
	Value buffer[freeze_max_buffer_size];
	short next;
};

#endif //FREEZE_H
