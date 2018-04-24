#include <coordinator.h>
#include <hardware.h>

#include <programs/program_change.h>

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
	active_program->load_program();
	hardware::getInstance().bypass = false;

	activate_task = Bela_createAuxiliaryTask(
			activate_callback, 
			BELA_AUDIO_PRIORITY - 1,
			"program_activator",
			nullptr);
}

void coordinator::activate_program(std::shared_ptr<program> new_prog)
{
	pending_programs.insert(pending_programs.begin(), new_prog);

	Bela_scheduleAuxiliaryTask(activate_task);
}

coordinator::coordinator()
{
	//TODO set active program (memory?)
	active_program = std::make_shared<program_change>();
}
