#ifndef DIRECT_POT
#define DIRECT_POT

#include <base/program.h>

#include <components/function_runner.h>
#include <components/constant.h>
#include <components/lfo.h>
#include <components/converter.h>

class direct_pot : public program 
{
public:
	direct_pot()
	{
		change_output = std::make_shared<function_runner<float>>([&](float val)->float {
				int iv = (int) val;
				if (iv != this->active_output) {
					this->set_output(iv);
				}
				return val;
		});
	}

	void load_program()
	{
		hardware& hw = hardware::getInstance();

		hw.pots[4]->maxv->register_emitter(constant<float>::make(2));
		hw.pots[4]->error->register_emitter(integer_pot_error);
		change_output->input->register_emitter(hw.pots[4]->value);
		hw.active_outputs.push_back(&(*change_output));

		conv.input->register_emitter(hw.pots[0]->value);

		osc.shape->register_emitter(conv.output);
		osc.frequency->register_emitter(hw.pots[1]->value);
		osc.duty_cycle->register_emitter(hw.pots[2]->value);

		hw.connect_jack(JACK_VOLUME, hw.pots[0]->value);
		hw.connect_jack(JACK_TRIGGER, hw.pots[3]->value);
		hw.connect_jack(JACK_MOD, hw.pots[1]->value);
		hw.connect_jack(JACK_CUTOFF, hw.pots[2]->value);
		hw.connect_jack(JACK_PITCH, hw.pots[5]->value);
	}
	void unload_program()
	{}

	void set_output(int v) 
	{
		hardware& hw = hardware::getInstance();
		if(v == 0) {
			hw.pots[0]->maxv->register_emitter(OneF);
			hw.pots[0]->error->register_emitter(default_pot_error);

			hw.connect_jack(JACK_VOLUME, hw.pots[0]->value);
			hw.connect_jack(JACK_TRIGGER, hw.pots[3]->value);
			hw.connect_jack(JACK_MOD, hw.pots[1]->value);
			hw.connect_jack(JACK_CUTOFF, hw.pots[2]->value);
			hw.connect_jack(JACK_PITCH, hw.pots[5]->value);
		} else {
			hw.pots[0]->maxv->register_emitter(lfo_shape_maxF);
			hw.pots[0]->error->register_emitter(integer_pot_error);

			hw.connect_jack(JACK_VOLUME, osc.value);
			hw.connect_jack(JACK_TRIGGER, osc.value);
			hw.connect_jack(JACK_MOD, osc.value);
			hw.connect_jack(JACK_CUTOFF, osc.value);
			hw.connect_jack(JACK_PITCH, osc.value);
		}
		active_output = v;
	}
private:
	lfo osc;
	int active_output;
	std::shared_ptr<function_runner<float>> change_output;
	converter<float, lfo_shape> conv;
};

#endif //DIRECT_POT
