#ifndef PRINTER_H
#define PRINTER_H

#include <component.h>

template <typename Input>
class printer: public component {
public:
	printer(std::string fmt)
	{
		output.setUpdateFunction([=](State *state)->Input {
			if(this->input.getValue(state) != this->prev_val){
				this->prev_val = this->input.getValue(state);
				rt_printf(fmt.c_str(), this->prev_val);
			}
			return this->prev_val;
		});
	}

	Receiver<Input> input;
	Emitter<Input> output;
protected:
	Input prev_val;
};

#endif //PRINTER_H
