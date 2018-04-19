#include <component.h>

Emitter::Emitter(Value default_val):
	lastElapsed(0),
	lastValidValue(default_val),
	updateFn(NULL)
{}

Value Emitter::getValue(State *state)
{
	if (state->context->audioFramesElapsed != lastElapsed){
		lastElapsed = state->context->audioFramesElapsed;
		if (updateFn == NULL) {
			rt_printf("ERROR: emitter has been started without update function\n");
		} else {
			lastValidValue = updateFn(state);
		}
	}
	return lastValidValue;
}

void Emitter::setUpdateFunction(std::function<Value(State *)> fn)
{
	updateFn = fn;
}

Receiver::Receiver(Value default_val, mixing_mode mix_mode):
	defaultVal(default_val)
{
	set_mixing_mode(mix_mode);
}

Value Receiver::getValue(State *state)
{
	Value val = defaultVal;
	for(auto e : sources) {
		val = mixer(val, e.getValue(state));
	}
	return val;
}

void Receiver::register_emitter(Emitter<Value> *emitter)
{
	sources.push_back(emitter);
}

void Receiver::clear_emitters()
{
	sources.clear();
}

void Receiver::set_mixing_mode(mixing_mode mix_mode)
{
	switch(mix_mode){
		case ADD:
			mixer = [](Value a, Value b)->Value {
				return a + b;
			};
			break;
		case MULTIPLY:
			mixer = [](Value a, Value b)->Value {
				return a * b;
			};
			break;
		case OR:
		default:
			mixer = [](Value a, Value b)->Value {
				return a | b;
			};
			break;
		case AND:
			mixer = [](Value a, Value b)->Value {
				return a & b;
			}
			break;
	}
}
