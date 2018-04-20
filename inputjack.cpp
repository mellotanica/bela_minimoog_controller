#include <jack.h>

inputJack::inputJack(unsigned short pin):
	value(Emitter<float>::make())
{
	value->setUpdateFunction([=](State *state)->float{
		return analogRead(state->context, state->analogFrame, pin);
	});	
}
