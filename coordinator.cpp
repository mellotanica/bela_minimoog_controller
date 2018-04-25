#include <coordinator.h>
#include <hardware.h>

#include <programs/program_change.h>

#define MODE_SWITCH_BYPASS	0
#define MODE_SWITCH_NORMAL	1
#define MODE_SWITCH_PROG_CHANGE	2

void activate_callback(void *arg)
{
	coordinator& coord = coordinator::getInstance();

	hardware::getInstance().bypass = true;
	
	std::shared_ptr<program> new_program;
	while(coord.pending_programs.size() > 0) {

		new_program = coord.pending_programs.back();
		coord.pending_programs.pop_back();

		if (coord.active_program) {
			coord.active_program->unload_program();
		}

		hardware::getInstance().active_outputs.clear();

		new_program->load_program();
		coord.active_program = new_program;
	}
	hardware::getInstance().bypass = false;
}

coordinator& coordinator::getInstance()
{
	static coordinator instance;
	return instance;
}

void coordinator::initialize()
{
	activate_task = Bela_createAuxiliaryTask(
			activate_callback, 
			BELA_AUDIO_PRIORITY - 1,
			"program_activator",
			nullptr);
	activate_program(active_program);
	mode_switch_handler->input->register_emitter(
			hardware::getInstance().switches[0]->position);
}

void coordinator::activate_program(std::shared_ptr<program> new_prog)
{
	pending_programs.insert(pending_programs.begin(), new_prog);

	Bela_scheduleAuxiliaryTask(activate_task);
}

coordinator::coordinator():
	mode_switch_pos((unsigned short)-1)
{
	mode_switch_handler = std::make_shared<function_runner<unsigned short>>([&](unsigned short pos)->unsigned short{
		if (pos != this->mode_switch_pos){
			switch(pos) {
				case MODE_SWITCH_BYPASS:
					hardware::getInstance().bypass = true;
					break;
				case MODE_SWITCH_NORMAL:
					if(this->mode_switch_pos == MODE_SWITCH_BYPASS){
						hardware::getInstance().bypass = false;
					} else {
						this->activate_program(p_program_change->get_selected_program());
					}
					break;
				case MODE_SWITCH_PROG_CHANGE:
					this->activate_program(p_program_change);
					break;
			}
			this->mode_switch_pos = pos;
		}
		return pos;
	});

	//TODO set active program (memory?)
	active_program = std::make_shared<program_change>();
}
