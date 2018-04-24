#ifndef PROGRAM_CHANGE_H
#define PROGRAM_CHANGE_H

#include <base/program.h>

// #include <programs/program_map.h>

#include <components/constant.h>
#include <components/comparator.h>
#include <components/function_runner.h>

#define BANK_POT 0
#define PROG_POT 1

class program_change: public program {
public:
	program_change()
	{
		update_bank = std::make_shared<function_runner<float>>([&](float val) {
			this->active_bank = (short) val;
		});

		update_program = std::make_shared<function_runner<float>>([&](float val) {
			this->active_program = (short) val;
		});
	}

	void load_program() 
	{
		hardware& hw = hardware::getInstance();
		int i;
		for(i = 0; i < 5; ++i) {
			comp[i].type->register_emitter(band_pass_comparator);
		}

		for(i = 0; i < 3; ++i) {
			comp[i].threshold_a->register_emitter(constant<float>::make(i));
			comp[i].threshold_b->register_emitter(constant<float>::make(i+1));
			comp[i].input->register_emitter(hw.pots[BANK_POT]->value);

			hw.leds[i*2]->state->register_emitter(comp[i].output);
		}

		for(i = 1; i < 3; ++i) {
			comp[i+2].threshold_a->register_emitter(constant<float>::make(i));
			comp[i+2].threshold_b->register_emitter(constant<float>::make(i+1));
			comp[i+2].input->register_emitter(hw.pots[PROG_POT]->value);

			hw.leds[(i-1)*2+1]->state->register_emitter(comp[i+2].output);
		}

		std::shared_ptr<constant<float>> threeF = constant<float>::make(3);
		hw.pots[BANK_POT]->minv->register_emitter(ZeroF);
		hw.pots[BANK_POT]->maxv->register_emitter(threeF);
		hw.pots[BANK_POT]->error->register_emitter(integer_pot_error);

		hw.pots[PROG_POT]->minv->register_emitter(ZeroF);
		hw.pots[PROG_POT]->maxv->register_emitter(threeF);
		hw.pots[PROG_POT]->error->register_emitter(integer_pot_error);

		update_bank->input->register_emitter(hw.pots[BANK_POT]->value);
		update_program->input->register_emitter(hw.pots[PROG_POT]->value);

		for(auto l: hw.leds) {
			hw.active_outputs.push_back(l);
		}

		hw.active_outputs.push_back(&(*update_bank));
		hw.active_outputs.push_back(&(*update_program));

	}

	void unload_program()
	{
	
	}
private:
	comparator<float> comp[5];

	std::shared_ptr<function_runner<float>> update_bank;
	std::shared_ptr<function_runner<float>> update_program;

	short active_bank;
	short active_program;
};


#endif //PROGRAM_CHANGE_H
