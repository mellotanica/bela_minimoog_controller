/*
 ____  _____ _        _    
| __ )| ____| |      / \   
|  _ \|  _| | |     / _ \  
| |_) | |___| |___ / ___ \ 
|____/|_____|_____/_/   \_\

The platform for ultra-low latency audio and sensor processing

http://bela.io

A project of the Augmented Instruments Laboratory within the
Centre for Digital Music at Queen Mary University of London.
http://www.eecs.qmul.ac.uk/~andrewm

(c) 2016 Augmented Instruments Laboratory: Andrew McPherson,
  Astrid Bin, Liam Donovan, Christian Heinrichs, Robert Jack,
  Giulio Moro, Laurel Pardue, Victor Zappi. All rights reserved.

The Bela software is distributed under the GNU Lesser General Public License
(LGPL 3.0), available here: https://www.gnu.org/licenses/lgpl-3.0.txt
*/


#include <Bela.h>

#include <led.h>
#include <pot.h>
#include <killswitch.h>
#include <switch.h>
// #include <jack.h>
// #include <lfo.h>
#include <constant.h>
#include <comparator.h>

#include <vector>

/*
 *  TEST
 */
#define POT_TEST
#define SWC_TEST
#define KSW_TEST
#define OSC_TEST

#if defined POT_TEST or defined SWC_TEST or defined KSW_TEST
int active_led = 0;
float duty_cycle = 0.5;
float pwm_period = 0.01;
#endif
/*
 *  TEST
 */


#define LEDS_COUNT 5
#define POTS_COUNT 6
#define SWITCHS_COUNT 1
#define KSWITCHS_COUNT 1
#define OUTPUT_JACKS_COUNT 5
#define INPUT_JACKS_COUNT 1
#define OSCILLATORS_COUNT 1

#define COMPONENTS_COUNT LEDS_COUNT + KSWITCHS_COUNT + POTS_COUNT + SWITCHS_COUNT// + INPUT_JACKS_COUNT + OUTPUT_JACKS_COUNT + OSCILLATORS_COUNT

// led(short pin);
led *leds[LEDS_COUNT] = {
	new led(P8_09),
	new led(P8_08),
	new led(P8_11),
	new led(P8_12),
	new led(P8_10),
};

//pot(short analogPin);
pot *pots[POTS_COUNT] = {
	new pot(3),
	new pot(5),
	new pot(1),
	new pot(2),
	new pot(4),
	new pot(0),
};

// killswitch(short pin, bool defaultState, unsigned int debounceMsecs = 2);
killswitch *killswitches[KSWITCHS_COUNT] = {
	new killswitch(P8_07, true, 0),
};

// switch_comp(short pin0, short pin2, switch_type type);
switch_comp *switches[SWITCHS_COUNT] = {
	new switch_comp(P8_15, P8_16, ON_OFF_ON),	
};

// inputJack(short analogPin);
// inputJack *inJacks[INPUT_JACKS_COUNT] = {
	// new inputJack(7),
// };

// outputJack(short analogPin);
// outputJack *outJacks[OUTPUT_JACKS_COUNT] = {
	// new outputJack(0),
	// new outputJack(1),
	// new outputJack(2),
	// new outputJack(3),
	// new outputJack(4),
// };

// lfo *oscillators[OSCILLATORS_COUNT] = {
	// new lfo(),
// };

component *components[COMPONENTS_COUNT];

std::vector<output*> outputs;

float gDigitalFramesPerAudioFrame, gAnalogFramesPerAudioFrame;
State gCurrentState;

// test


bool setup(BelaContext *context, void *userData)
{
	// Check if analog channels are enabled
	if(context->analogFrames == 0 || context->analogFrames > context->audioFrames) {
		rt_printf("Error: this example needs analog enabled, with 4 or 8 channels\n");
		return false;
	}
	
	int i, c = 0;

	// Add instances to components	
	for (i = 0; i < LEDS_COUNT; ++i) {
		components[c++] = leds[i];
		outputs.push_back(leds[i]);
	}
	for (i = 0; i < KSWITCHS_COUNT; ++i) {
		components[c++] = killswitches[i];
	}
	for (i = 0; i < POTS_COUNT; ++i) {
		components[c++] = pots[i];
	}
	for (i = 0; i < SWITCHS_COUNT; ++i) {
		components[c++] = switches[i];
	}
	// for (i = 0; i < INPUT_JACKS_COUNT; ++i) {
		// components[c++] = inJacks[i];
	// }
	// for (i = 0; i < OUTPUT_JACKS_COUNT; ++i) {
		// components[c++] = outJacks[i];
	// }
	// for (i = 0; i < OSCILLATORS_COUNT; ++i) {
		// components[c++] = oscillators[i];
	// }
	
	// Setup components
	for(c = 0; c < COMPONENTS_COUNT; ++c) {
		components[c]->setup(context, userData);
	}
	
	gDigitalFramesPerAudioFrame = context->digitalFrames / context->audioFrames;
	gAnalogFramesPerAudioFrame = context->analogFrames / context->audioFrames;
	
/*
 *  TEST
 */
 
	leds[0]->pwm_period.register_emitter(&(oneF.value));
	leds[0]->pwm_duty_cycle.register_emitter(&(oneF.value));
	leds[0]->state.register_emitter(&(killswitches[0]->state));
	
	auto threeF = constant<float>(3);
	
	pots[0]->minv.register_emitter(&(oneF.value));
	pots[0]->maxv.register_emitter(&(threeF.value));
	pots[0]->error.register_emitter(&(integer_pot_error.value));
	
	pots[1]->minv.register_emitter(&(zeroF.value));
	pots[1]->maxv.register_emitter(&(oneF.value));
	leds[1]->pwm_duty_cycle.register_emitter(&(pots[1]->value));
	
	auto min_period = new constant<float>(0.5);
	auto max_period = new constant<float>(0.01);
	
	pots[2]->minv.register_emitter(&(min_period->value));
	pots[2]->maxv.register_emitter(&(max_period->value));
	leds[1]->pwm_period.register_emitter(&(pots[2]->value));

	leds[1]->state.register_emitter(&(True.value));
	
	// static comparator<float> comps[] = {
		// comparator<float>(BAND_PASS),
		// comparator<float>(BAND_PASS),
		// comparator<float>(BAND_PASS),
	// };
	
	// static constant<float> step_a[] = {
		// constant<float>(1),
		// constant<float>(2),
		// constant<float>(3),
	// };
	
	// static constant<float> step_b[] = {
		// constant<float>(2),
		// constant<float>(3),
		// constant<float>(4),
	// };
	
	// for(i = 0; i < 3; i++){
		// comps[i].threshold_a.register_emitter(&(step_a[i].value));
		// comps[i].threshold_b.register_emitter(&(step_b[i].value));
		// comps[i].input.register_emitter(&(pots[0]->value));
		
		// leds[i+1]->state.register_emitter(&(comps[i].output));
		// leds[i+1]->pwm_duty_cycle.register_emitter(&(pots[1]->value));
		// leds[i+1]->pwm_period.register_emitter(&(pots[2]->value));
	// }
/*
 *  TEST
 */

	return true;
}

void render(BelaContext *context, void *userData)
{
	gCurrentState.context = context;
	gCurrentState.userData = userData;
	
	// the audioSampleRate is the leading value, so we progress at that frequency
	for(gCurrentState.audioFrame = 0; gCurrentState.audioFrame < context->audioFrames; ++gCurrentState.audioFrame) {
		gCurrentState.analogFrame = gCurrentState.audioFrame * gAnalogFramesPerAudioFrame;
		gCurrentState.digitalFrame = gCurrentState.audioFrame * gDigitalFramesPerAudioFrame;

		for (auto o : outputs) {
			o->render(&gCurrentState);
		}
	}
}

void cleanup(BelaContext *context, void *userData)
{
	for(int c = 0; c < COMPONENTS_COUNT; ++c) {
		components[c]->cleanup(context, userData);
	}
}

