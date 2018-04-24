#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <Bela.h>

#include <base/program.h>
#include <components/function_runner.h>

#include <vector>
#include <memory>

class coordinator {
public:
	static coordinator& getInstance();

	void initialize();

	void activate_program(std::shared_ptr<program> new_program);

	std::shared_ptr<function_runner<unsigned short>> mode_switch_handler;
private:
	std::shared_ptr<program> active_program;
	std::vector<std::shared_ptr<program>> pending_programs;
	
	AuxiliaryTask activate_task;

	unsigned short mode_switch_pos;

	coordinator();

	friend void activate_callback(void *arg);
public:
	coordinator(coordinator const&) = delete;
	void operator=(coordinator const&) = delete;
};

#endif //COORDINATOR_H
