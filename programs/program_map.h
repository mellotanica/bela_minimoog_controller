#ifndef PROGRAM_MAP_H
#define PROGRAM_MAP_H

#include <base/program.h>

#include <vector>
#include <memory>

#include <programs/led_test.h>

static std::vector<std::shared_ptr<program>> p_banks[] = {
	{
		std::make_shared<led_test>(),
		std::make_shared<led_test>(),
		std::make_shared<led_test>(),
	},
	{
		std::make_shared<led_test>(),
		std::make_shared<led_test>(),
		std::make_shared<led_test>(),
	},
	{
		std::make_shared<led_test>(),
		std::make_shared<led_test>(),
		std::make_shared<led_test>(),
	},
};


#endif //PROGRAM_MAP_H
