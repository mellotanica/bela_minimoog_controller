#include <coordinator.h>
#include <hardware.h>

coordinator& coordinator::getInstance()
{
	static coordinator instance;
	return instance;
}

void coordinator::activate_program(std::shared_ptr<program> new_program)
{
	hardware::getInstance().bypass = true;
	if (active_program) {
		active_program->unload_program();
	}
	hardware::getInstance().active_outputs.clear();
	new_program->load_program();
	active_program = new_program;
	hardware::getInstance().bypass = false;
}

coordinator::coordinator()
{}
