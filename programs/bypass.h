#ifndef BYPASS_PROGRAM_H
#define BYPASS_PROGRAM_H

#include <base/program.h>

#include <components/constant.h>

class bypass_program : public program {
public:
	void load_program()
	{}

	void unload_program()
	{}
};

static auto p_bypass_prgram = std::make_shared<bypass_program>();

#endif
