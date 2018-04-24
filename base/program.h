#ifndef PROGRAM_H
#define PROGRAM_H

class program {
public:
	virtual void load_program() = 0;
	virtual void unload_program() = 0;
};

#endif //PROGRAM_H
