#ifndef PROGRAM_MAP_H
#define PROGRAM_MAP_H

#include <base/program.h>

#include <vector>
#include <memory>

#include <programs/led_test.h>
#include <programs/tremolo_sync.h>
#include <programs/adsr_test.h>
#include <programs/adsr_stk_test.h>
#include <programs/timer_test.h>

static std::vector<std::shared_ptr<program>> p_banks[] = {
	{
		std::make_shared<tremolo_sync>(),
		std::make_shared<tremolo_sync>(),
		std::make_shared<tremolo_sync>(),
	},
	{
		std::make_shared<adsr_test>(),
		std::make_shared<adsr_stk_test>(),
		std::make_shared<adsr_test>(),
	},
	{
		std::make_shared<timer_test>(),
		std::make_shared<timer_test>(),
		std::make_shared<led_test>(),
	},
};


#endif //PROGRAM_MAP_H
