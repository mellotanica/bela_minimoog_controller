#ifndef ARPEGGIATOR_H
#define ARPEGGIATOR_H

#include <base/program.h>

#include <hardware.h>

#include <components/constant.h>
#include <components/lfo.h>

class arpeggiator : public program {
public:
	void load_program()
	{
	
	}

	void unload_program()
	{
	}

protected:
	lfo osc;
};


#endif //ARPEGGIATOR_H
