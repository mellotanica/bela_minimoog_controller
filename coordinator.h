#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <base/program.h>

#include <memory>

class coordinator {
public:
	static coordinator& getInstance();

	void activate_program(std::shared_ptr<program> new_program);

	std::shared_ptr<program> active_program;
	
private:
	coordinator();

public:
	coordinator(coordinator const&) = delete;
	void operator=(coordinator const&) = delete;
};

#endif //COORDINATOR_H
