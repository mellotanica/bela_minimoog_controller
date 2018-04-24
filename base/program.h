#ifndef PROGRAM_H
#define PROGRAM_H

class program {
public:
	/** this function is called when the program is loaded, it should 
	 * connect all the needed elements and register the active outputs
	 */
	virtual void load_program() = 0;

	/** this function should perform cleanup when unloading the program
	 */
	virtual void unload_program() = 0;
};

#endif //PROGRAM_H
