#include <jack.h>

inputJack::inputJack(unsigned short pin):
	value(std::make_shared<Emitter<float>>())
{
	value->setUpdateFunction([=](State *state)->float{
		return analogRead(state->context, state->analogFrame, pin);
	});	
}
