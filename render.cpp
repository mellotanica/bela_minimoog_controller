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
#include <printer.h>

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
 
	leds[0]->pwm_period.register_emitter(&oneF);
	leds[0]->pwm_duty_cycle.register_emitter(&oneF);
	leds[0]->state.register_emitter(&(killswitches[0]->state));
	
	pots[0]->minv.register_emitter(new constant<float>(1));
	pots[0]->maxv.register_emitter(new constant<float>(4));
	pots[0]->error.register_emitter(&integer_pot_error);
	
	auto pot0_pr = new printer<float>("Pots[0]: %f\n");
	pot0_pr->input.register_emitter(&(pots[0]->value));

	pots[1]->minv.register_emitter(&zeroF);
	pots[1]->maxv.register_emitter(&oneF);
	
	auto pot1_pr = new printer<float>("Pots[1]: %f\n");
	pot1_pr->input.register_emitter(&(pots[1]->value));

	pots[2]->minv.register_emitter(new constant<float>(0.5));
	pots[2]->maxv.register_emitter(new constant<float>(0.01));

	auto pot2_pr = new printer<float>("Pots[2]: %f\n");
	pot2_pr->input.register_emitter(&(pots[2]->value));

	for(i = 1; i < 4; i++){
		auto comp = new comparator<float>(BAND_PASS);

		comp->threshold_a.register_emitter(new constant<float>(i));
		comp->threshold_b.register_emitter(new constant<float>(i+1));
		comp->input.register_emitter(&(pot0_pr->output));

		std::string log = "comp["+std::to_string(i)+"]: %d\n";

		auto comp_pr = new printer<bool>(log);

		comp_pr->input.register_emitter(&(comp->output));

		leds[i]->state.register_emitter(&(comp_pr->output));
		leds[i]->pwm_duty_cycle.register_emitter(&(pot1_pr->output));
		leds[i]->pwm_period.register_emitter(&(pot2_pr->output));
	}
	
	
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

