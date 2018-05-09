#ifndef ADSR_H
#define ADSR_H

#include <base/component.h>

#include <components/constant.h>

enum adsr_state
{
	IDLE = 0,
	ATTACK,
	DECAY,
	SUSTAIN,
	RELEASE,
	INVALID,
};

float __adsr_linear(float phase, float start, float target);
float __adsr_exponential(float phase, float start, float target);
float __adsr_logaritmic(float phase, float start, float target);

typedef float (*adsr_shape_fun)(float, float, float);

static auto adsr_default_mintime = constant<float>::make(0.01);
static auto adsr_default_maxtime = constant<float>::make(5);
static auto adsr_default_time = constant<float>::make(0.1);
static auto adsr_default_sustain = constant<float>::make(0.7);

static auto adsr_linear = constant<adsr_shape_fun>::make(__adsr_linear);
static auto adsr_logaritmic = constant<adsr_shape_fun>::make(__adsr_logaritmic);
static auto adsr_exponential = constant<adsr_shape_fun>::make(__adsr_exponential);

class adsr : public component {
public:
	adsr(EmitterP<float> attack = adsr_default_time,
		EmitterP<float> decay = adsr_default_time,
		EmitterP<float> sustain = adsr_default_sustain,
		EmitterP<float> release = adsr_default_time,
		EmitterP<float> attack_level = OneF,
		EmitterP<bool> soft_reset = True,
		EmitterP<adsr_shape_fun> attack_function = adsr_linear,
		EmitterP<adsr_shape_fun> decay_function = adsr_linear,
		EmitterP<adsr_shape_fun> release_function = adsr_linear);

	void reset();

	ReceiverP<float> attack;
	ReceiverP<float> decay;
	ReceiverP<float> sustain;
	ReceiverP<float> release;
	ReceiverP<float> attack_level;

	ReceiverP<bool> gate;

	ReceiverP<bool> soft_reset;

	ReceiverP<adsr_shape_fun> attack_function;
	ReceiverP<adsr_shape_fun> decay_function;
	ReceiverP<adsr_shape_fun> release_function;

	EmitterP<float> value;
	EmitterP<bool> inactive;

private:
	adsr_state active_state;

	unsigned long step;
	bool last_gate;

	float lastV;
	float phaseStart;

	float evaluate(State *state);
};

#endif //ADSR_H
